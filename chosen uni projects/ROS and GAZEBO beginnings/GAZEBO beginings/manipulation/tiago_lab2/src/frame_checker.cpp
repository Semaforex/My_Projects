#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/pose.hpp>
#include <geometry_msgs/msg/transform_stamped.hpp>

#include <moveit/move_group_interface/move_group_interface.h>
#include <tf2_ros/transform_listener.h>
#include <tf2_ros/buffer.h>
#include <tf2/LinearMath/Transform.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>

#include <gazebo_msgs/srv/get_entity_state.hpp>

static tf2::Transform poseToTransform(const geometry_msgs::msg::Pose& p) {
  tf2::Transform T;
  T.setOrigin(tf2::Vector3(p.position.x, p.position.y, p.position.z));
  tf2::Quaternion q(p.orientation.x, p.orientation.y, p.orientation.z, p.orientation.w);
  T.setRotation(q);
  return T;
}

static tf2::Transform tsToTransform(const geometry_msgs::msg::TransformStamped& ts) {
  tf2::Transform T;
  T.setOrigin(tf2::Vector3(
      ts.transform.translation.x,
      ts.transform.translation.y,
      ts.transform.translation.z));
  tf2::Quaternion q(
      ts.transform.rotation.x,
      ts.transform.rotation.y,
      ts.transform.rotation.z,
      ts.transform.rotation.w);
  T.setRotation(q);
  return T;
}

int main(int argc, char** argv)
{
  rclcpp::init(argc, argv);
  auto node = rclcpp::Node::make_shared("frame_checker");

  // ---- Parametry (możesz nadpisać z CLI)
  auto planning_group         = node->declare_parameter<std::string>("planning_group", "arm_torso");
  auto gazebo_entity_F        = node->declare_parameter<std::string>("gazebo_entity_F", "wrist_ft_link"); // F w Gazebo
  auto gazebo_reference_frame = node->declare_parameter<std::string>("gazebo_reference_frame", "base_footprint"); // G w Gazebo
  auto tf_reference_frame_G   = node->declare_parameter<std::string>("tf_reference_frame_G", "base_footprint"); // G w TF (często bez "tiago::")

  // ---- MoveIt: B i E
  moveit::planning_interface::MoveGroupInterface mgi(node, planning_group);
  const std::string B = mgi.getPlanningFrame();
  const std::string E = mgi.getEndEffectorLink();

  RCLCPP_INFO(node->get_logger(), "Planning frame (B): %s", B.c_str());
  RCLCPP_INFO(node->get_logger(), "End effector (E):  %s", E.c_str());
  RCLCPP_INFO(node->get_logger(), "Gazebo F entity:   %s", gazebo_entity_F.c_str());
//   RCLCPP_INFO(node->get_logger(), "Gazebo ref (G):    %s", gazebo_reference_frame.c_str());
//   RCLCPP_INFO(node->get_logger(), "TF ref for G:      %s", tf_reference_frame_G.c_str());

  // ---- TF
  tf2_ros::Buffer tf_buffer(node->get_clock());
  tf2_ros::TransformListener tf_listener(tf_buffer);

// zakładamy: F jest w TF
auto T_B_E_msg = tf_buffer.lookupTransform(B, E, tf2::TimePointZero);
  RCLCPP_INFO(node->get_logger(), "ok: %s", B.c_str());

auto T_B_F_msg = tf_buffer.lookupTransform(B, gazebo_entity_F, tf2::TimePointZero);

tf2::Transform T_B_E = tsToTransform(T_B_E_msg);
tf2::Transform T_B_F = tsToTransform(T_B_F_msg);

tf2::Transform T_E_F = T_B_E.inverse() * T_B_F;


  // ---- Wynik
  tf2::Vector3 p = T_E_F.getOrigin();
  tf2::Quaternion q = T_E_F.getRotation();

  RCLCPP_INFO(node->get_logger(),
              "=== ^E T_F ===\n"
              "p: [%.6f, %.6f, %.6f]\n"
              "q: [x=%.6f, y=%.6f, z=%.6f, w=%.6f]",
              p.x(), p.y(), p.z(), q.x(), q.y(), q.z(), q.w());

  rclcpp::shutdown();
  return 0;
}
