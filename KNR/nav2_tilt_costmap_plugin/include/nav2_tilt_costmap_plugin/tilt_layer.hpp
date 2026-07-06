/*********************************************************************
 *
 * Software License Agreement (BSD License)
 *
 *********************************************************************/
#ifndef TILT_LAYER_HPP_
#define TILT_LAYER_HPP_

#include <mutex>
#include <vector>
#include <utility>
#include <memory>
#include <unordered_map>
#include <limits>

#include "rclcpp/rclcpp.hpp"
#include "nav2_costmap_2d/layer.hpp"
#include "nav2_costmap_2d/layered_costmap.hpp"
#include "nav2_costmap_2d/costmap_layer.hpp"

#include "nav_msgs/msg/grid_cells.hpp"
#include "geometry_msgs/msg/point.hpp"

#include "tf2_ros/buffer.h"
#include "tf2_ros/transform_listener.h"
#include "geometry_msgs/msg/transform_stamped.hpp"

#include "sensor_msgs/msg/point_cloud2.hpp"

namespace nav2_tilt_costmap_plugin
{

class TiltLayer : public nav2_costmap_2d::CostmapLayer
{
public:
  TiltLayer();

  virtual void onInitialize();
  virtual void updateBounds(
    double robot_x, double robot_y, double robot_yaw, double * min_x,
    double * min_y,
    double * max_x,
    double * max_y);
  virtual void updateCosts(
    nav2_costmap_2d::Costmap2D & master_grid,
    int min_i, int min_j, int max_i, int max_j);

  virtual void reset()
  {
    resetMaps();
    current_ = false;
    need_recalculation_ = true;
  }

  virtual void onFootprintChanged();

  virtual bool isClearable() {return false;}

  virtual void matchSize() override;

private:
  void pointCloudCallback(const sensor_msgs::msg::PointCloud2::SharedPtr msg);

  void getStats(std::vector<float> & zs, float & mean, float & min_z, float & max_z);

  rclcpp::Publisher<nav_msgs::msg::GridCells>::SharedPtr debug_pub_;

  double last_min_x_, last_min_y_, last_max_x_, last_max_y_;
  double min_x_, min_y_, max_x_, max_y_;

  bool need_recalculation_;
  bool enabled_;
  bool rolling_window_;

  double alpha_max_;        
  double alpha_min_;        
  double max_height_;  
  double min_height_;     
  double z_max_filter_;
  int    min_points_per_cell_;
  int    dh_radius_;
  std::string topic_;
  std::string frame_;

  std::shared_ptr<tf2_ros::Buffer> tf_buffer_;
  std::shared_ptr<tf2_ros::TransformListener> tf_listener_;

  rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr pc_sub_;

  std::mutex data_mutex_;
};

}  // namespace nav2_tilt_costmap_plugin

#endif  // TILT_LAYER_HPP_