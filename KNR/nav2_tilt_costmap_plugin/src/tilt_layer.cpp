/*********************************************************************
 *
 * Software License Agreement (BSD License)
 *
 *********************************************************************/
#include "nav2_tilt_costmap_plugin/tilt_layer.hpp"
#include "nav2_costmap_2d/costmap_math.hpp"
#include "nav2_costmap_2d/footprint.hpp"
#include "sensor_msgs/point_cloud2_iterator.hpp"
#include "tf2_sensor_msgs/tf2_sensor_msgs.hpp"
#include "rclcpp/parameter_events_filter.hpp"
#include "nav_msgs/msg/grid_cells.hpp"
#include "geometry_msgs/msg/point.hpp"

#include <cmath>
#include <algorithm>
#include <limits>
#include <unordered_map>

// using nav2_costmap_2d::LETHAL_OBSTACLE;
// using nav2_costmap_2d::FREE_SPACE;
using nav2_costmap_2d::NO_INFORMATION;

int LETHAL_OBSTACLE = 254;
int FREE_SPACE = 0;

namespace nav2_tilt_costmap_plugin
{

TiltLayer::TiltLayer()
: last_min_x_(-std::numeric_limits<double>::max()),
  last_min_y_(-std::numeric_limits<double>::max()),
  last_max_x_(std::numeric_limits<double>::max()),
  last_max_y_(std::numeric_limits<double>::max()),
  min_x_(std::numeric_limits<double>::max()),
  min_y_(std::numeric_limits<double>::max()),
  max_x_(std::numeric_limits<double>::lowest()),
  max_y_(std::numeric_limits<double>::lowest())
{
}

void TiltLayer::getStats(std::vector<float> & zs, float & mean, float & min_z, float & max_z)
{
  if (zs.empty()) {
    mean = 0.0f; min_z = 0.0f; max_z = 0.0f;
    return;
  }

  std::sort(zs.begin(), zs.end());
  size_t n = zs.size();

  size_t drop = (n >= 5) ? static_cast<size_t>(n * 0.1) : 0;
  size_t start = drop;
  size_t end = n - drop;

  min_z = zs[start];
  max_z = zs[end - 1];

  double sum = 0;
  for (size_t i = start; i < end; ++i) {
    sum += zs[i];
  }
  mean = static_cast<float>(sum / (end - start));
}

void TiltLayer::onInitialize()
{
  auto node = node_.lock();

  node->declare_parameter(name_ + "." + "enabled",             rclcpp::ParameterValue(true));
  node->declare_parameter(name_ + "." + "topic",               rclcpp::ParameterValue(std::string("/nvblox/mesh_pointcloud")));
  node->declare_parameter(name_ + "." + "alpha_min",           rclcpp::ParameterValue(5.0));
  node->declare_parameter(name_ + "." + "alpha_max",           rclcpp::ParameterValue(15.0));
  node->declare_parameter(name_ + "." + "max_height",          rclcpp::ParameterValue(0.25));
  node->declare_parameter(name_ + "." + "min_height",          rclcpp::ParameterValue(0.15));
  node->declare_parameter(name_ + "." + "z_max_filter",        rclcpp::ParameterValue(2.0));
  node->declare_parameter(name_ + "." + "min_points_per_cell", rclcpp::ParameterValue(1));
  node->declare_parameter(name_ + "." + "frame",               rclcpp::ParameterValue(std::string("odom_2d")));
  node->declare_parameter(name_ + "." + "dh_radius",      rclcpp::ParameterValue(2));

  node->get_parameter(name_ + "." + "enabled",             enabled_);
  node->get_parameter(name_ + "." + "topic",               topic_);
  node->get_parameter(name_ + "." + "alpha_min",           alpha_min_);
  node->get_parameter(name_ + "." + "alpha_max",           alpha_max_);
  node->get_parameter(name_ + "." + "max_height",          max_height_);
  node->get_parameter(name_ + "." + "min_height",          min_height_);
  node->get_parameter(name_ + "." + "z_max_filter",        z_max_filter_);
  node->get_parameter(name_ + "." + "min_points_per_cell", min_points_per_cell_);
  node->get_parameter(name_ + "." + "frame", frame_);
  node->get_parameter(name_ + "." + "dh_radius", dh_radius_);

  tf_buffer_   = std::make_shared<tf2_ros::Buffer>(node->get_clock());
  tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);

  pc_sub_ = node->create_subscription<sensor_msgs::msg::PointCloud2>(
    topic_, rclcpp::SensorDataQoS(),
    std::bind(&TiltLayer::pointCloudCallback, this, std::placeholders::_1));

  debug_pub_ = node->create_publisher<nav_msgs::msg::GridCells>("tilt_layer_debug", 10);

  default_value_ = NO_INFORMATION;
  matchSize();
  current_ = true;
  need_recalculation_ = false;
  rolling_window_ = layered_costmap_->isRolling();
}

void TiltLayer::matchSize()
{
  nav2_costmap_2d::Costmap2D * master = layered_costmap_->getCostmap();
  resizeMap(master->getSizeInCellsX(), master->getSizeInCellsY(),
            master->getResolution(),
            master->getOriginX(), master->getOriginY());
  resetMaps();  //added to prevent random init. Could not work if plugin initiates after static global (that should never happen)
}

void TiltLayer::pointCloudCallback(const sensor_msgs::msg::PointCloud2::SharedPtr msg)
{
  if (!enabled_) return;

  geometry_msgs::msg::TransformStamped transform_stamped;
  try {
    transform_stamped = tf_buffer_->lookupTransform(
      frame_, msg->header.frame_id, msg->header.stamp, rclcpp::Duration::from_seconds(0.5));  //long time tolerance required for safe operation
  } catch (const tf2::TransformException & ex) {
    RCLCPP_WARN(node_.lock()->get_logger(), "TF Error w TiltLayer: %s", ex.what());
    return;
  }

  sensor_msgs::msg::PointCloud2 global_cloud;
  tf2::doTransform(*msg, global_cloud, transform_stamped);

  nav2_costmap_2d::Costmap2D * costmap = layered_costmap_->getCostmap();
  const double resolution = costmap->getResolution();

  std::unordered_map<unsigned int, std::vector<float>> cell_zs;

  unsigned int min_mx = std::numeric_limits<unsigned int>::max();
  unsigned int min_my = std::numeric_limits<unsigned int>::max();
  unsigned int max_mx = 0;
  unsigned int max_my = 0;

  sensor_msgs::PointCloud2ConstIterator<float> iter_x(global_cloud, "x");
  sensor_msgs::PointCloud2ConstIterator<float> iter_y(global_cloud, "y");
  sensor_msgs::PointCloud2ConstIterator<float> iter_z(global_cloud, "z");

  for (; iter_x != iter_x.end(); ++iter_x, ++iter_y, ++iter_z) {
    const float px = *iter_x, py = *iter_y, pz = *iter_z;

    if (std::isnan(px) || std::isnan(py) || std::isnan(pz)) continue;
    if (pz > z_max_filter_) continue;

    unsigned int mx, my;
    if (!costmap->worldToMap(px, py, mx, my)) continue;

    min_mx = std::min(min_mx, mx);
    max_mx = std::max(max_mx, mx);
    min_my = std::min(min_my, my);
    max_my = std::max(max_my, my);

    unsigned int index = costmap->getIndex(mx, my);
    cell_zs[index].push_back(pz);
  }

  if (min_mx > max_mx || min_my > max_my) {
    return;
  }

  int width = max_mx - min_mx + 1;
  int height = max_my - min_my + 1;

  std::vector<float> z_grid(width * height, std::numeric_limits<float>::quiet_NaN());
  std::vector<float> z_min_grid(width * height, std::numeric_limits<float>::quiet_NaN());
  std::vector<float> z_max_grid(width * height, std::numeric_limits<float>::quiet_NaN());

  for (auto & [idx, zs] : cell_zs) {
    if (zs.size() < static_cast<size_t>(min_points_per_cell_)) continue;
    float mean, min_z, max_z;
    getStats(zs, mean, min_z, max_z);

    unsigned int mx, my;
    costmap->indexToCells(idx, mx, my);
    int local_idx = (my - min_my) * width + (mx - min_mx);
    z_grid[local_idx] = mean;
    z_min_grid[local_idx] = min_z;
    z_max_grid[local_idx] = max_z;
  }

  std::vector<float> interp_grid = z_grid;
  int r = dh_radius_;
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      if (std::isnan(z_grid[y * width + x])) {
        float sum = 0;
        int count = 0;
        for (int dy = -r; dy <= r; ++dy) {
          for (int dx = -r; dx <= r; ++dx) {
            int nx = x + dx;
            int ny = y + dy;
            if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
              float val = z_grid[ny * width + nx];
              if (!std::isnan(val)) {
                sum += val;
                count++;
              }
            }
          }
        }
        if (count > 0) {
          float interpolated_mean = sum / count;
          interp_grid[y * width + x] = interpolated_mean;
          z_min_grid[y * width + x] = interpolated_mean;
          z_max_grid[y * width + x] = interpolated_mean;
        }
      }
    }
  }

  nav_msgs::msg::GridCells debug_msg;
  debug_msg.header.frame_id = frame_;
  debug_msg.header.stamp    = node_.lock()->now();
  debug_msg.cell_width      = resolution;
  debug_msg.cell_height     = resolution;

  std::lock_guard<std::mutex> lock(data_mutex_);

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      float z_val = interp_grid[y * width + x];
      if (std::isnan(z_val)) continue;

      int nan_count = 0;
      for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
          int nx = x + dx;
          int ny = y + dy;
          if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
            if (std::isnan(z_grid[ny * width + nx])) nan_count++;
          } else {
            nan_count++;
          }
        }
      }
      if (nan_count > 3) continue;

      unsigned int mx = x + min_mx;
      unsigned int my = y + min_my;

      float local_min = z_min_grid[y * width + x];
      float local_max = z_max_grid[y * width + x];

      for (int dy = -r; dy <= r; ++dy) {
        for (int dx = -r; dx <= r; ++dx) {
          int nx = x + dx;
          int ny = y + dy;
          if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
            if (!std::isnan(z_min_grid[ny * width + nx])) {
              local_min = std::min(local_min, z_min_grid[ny * width + nx]);
              local_max = std::max(local_max, z_max_grid[ny * width + nx]);
            }
          }
        }
      }

      float dh = local_max - local_min;
      unsigned char final_cost = FREE_SPACE;
      float cost_scalar = 1.0f;
      if (dh < min_height_) {
          final_cost = FREE_SPACE;
      } else {

        if (dh < max_height_) {
          cost_scalar = (dh - min_height_) / (max_height_ - min_height_);
        }

        float dz_dx = 0.0f;
        float dz_dy = 0.0f;

        if (x > 0 && x < width - 1 && !std::isnan(interp_grid[y * width + x + 1]) && !std::isnan(interp_grid[y * width + x - 1])) {
          dz_dx = (interp_grid[y * width + x + 1] - interp_grid[y * width + x - 1]) / (2.0 * resolution);
        } else if (x < width - 1 && !std::isnan(interp_grid[y * width + x + 1])) {
          dz_dx = (interp_grid[y * width + x + 1] - z_val) / resolution;
        } else if (x > 0 && !std::isnan(interp_grid[y * width + x - 1])) {
          dz_dx = (z_val - interp_grid[y * width + x - 1]) / resolution;
        }

        if (y > 0 && y < height - 1 && !std::isnan(interp_grid[(y + 1) * width + x]) && !std::isnan(interp_grid[(y - 1) * width + x])) {
          dz_dy = (interp_grid[(y + 1) * width + x] - interp_grid[(y - 1) * width + x]) / (2.0 * resolution);
        } else if (y < height - 1 && !std::isnan(interp_grid[(y + 1) * width + x])) {
          dz_dy = (interp_grid[(y + 1) * width + x] - z_val) / resolution;
        } else if (y > 0 && !std::isnan(interp_grid[(y - 1) * width + x])) {
          dz_dy = (z_val - interp_grid[(y - 1) * width + x]) / resolution;
        }

        float grad = std::sqrt(dz_dx * dz_dx + dz_dy * dz_dy);
        float alpha_deg = std::atan(grad) * 180.0 / M_PI;

        if (alpha_deg >= alpha_max_) {
          final_cost = static_cast<unsigned char>(FREE_SPACE + cost_scalar * (LETHAL_OBSTACLE - FREE_SPACE));
        } else if (alpha_deg > alpha_min_) {
          float t = (alpha_deg - alpha_min_) / (alpha_max_ - alpha_min_);
          final_cost = static_cast<unsigned char>(FREE_SPACE + t * cost_scalar * (LETHAL_OBSTACLE - FREE_SPACE));
        }
      }

      double wx, wy;
      costmap->mapToWorld(mx, my, wx, wy);

      unsigned int map_mx, map_my;
      if (!worldToMap(wx, wy, map_mx, map_my)) continue;

      setCost(map_mx, map_my, final_cost);

      min_x_ = std::min(min_x_, wx);
      min_y_ = std::min(min_y_, wy);
      max_x_ = std::max(max_x_, wx);
      max_y_ = std::max(max_y_, wy);

      if (final_cost > FREE_SPACE) {
        geometry_msgs::msg::Point p;
        p.x = wx; p.y = wy; p.z = 0.0;
        debug_msg.cells.push_back(p);
      }
    }
  }

  debug_pub_->publish(debug_msg);
  need_recalculation_ = true;
}

void TiltLayer::updateBounds(
  double robot_x, double robot_y, double /*robot_yaw*/, double * min_x,
  double * min_y, double * max_x, double * max_y)
{
  if (!enabled_) return;

  if (rolling_window_) {
    updateOrigin(robot_x - getSizeInMetersX() / 2.0, robot_y - getSizeInMetersY() / 2.0);
  }

  useExtraBounds(min_x, min_y, max_x, max_y);

  std::lock_guard<std::mutex> lock(data_mutex_);
  if (min_x_ > max_x_ || min_y_ > max_y_) return;

  *min_x = std::min(*min_x, min_x_);
  *min_y = std::min(*min_y, min_y_);
  *max_x = std::max(*max_x, max_x_);
  *max_y = std::max(*max_y, max_y_);

  min_x_ = std::numeric_limits<double>::max();
  min_y_ = std::numeric_limits<double>::max();
  max_x_ = std::numeric_limits<double>::lowest();
  max_y_ = std::numeric_limits<double>::lowest();
}

void TiltLayer::onFootprintChanged()
{
  need_recalculation_ = true;
}

void TiltLayer::updateCosts(
  nav2_costmap_2d::Costmap2D & master_grid, int min_i, int min_j, int max_i, int max_j)
{
  if (!enabled_) return;

  std::lock_guard<std::mutex> lock(data_mutex_);
  unsigned char * master_array = master_grid.getCharMap();
  unsigned char * local_array = costmap_;

  int span = master_grid.getSizeInCellsX();

  for (int j = min_j; j < max_j; j++) {
    for (int i = min_i; i < max_i; i++) {
      int index = j * span + i;
      unsigned char cost = local_array[index];

      if (cost == NO_INFORMATION) continue;

      unsigned char old_cost = master_array[index];

      if (cost == FREE_SPACE || old_cost == NO_INFORMATION || old_cost < cost) {
        master_array[index] = cost;
      }
    }
  }

  need_recalculation_ = false;
}

}  // namespace nav2_tilt_costmap_plugin

#include "pluginlib/class_list_macros.hpp"
PLUGINLIB_EXPORT_CLASS(nav2_tilt_costmap_plugin::TiltLayer, nav2_costmap_2d::Layer)