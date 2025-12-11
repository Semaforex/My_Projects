cmake_minimum_required(VERSION 3.5)
project(tiago_lab2)

# Minimal CMakeLists for a launch/config-only package using ament_cmake
find_package(ament_cmake REQUIRED)

# Install launch files, configs and worlds into share/<package>
install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}
)

install(DIRECTORY config
  DESTINATION share/${PROJECT_NAME}
)

install(DIRECTORY worlds
  DESTINATION share/${PROJECT_NAME}
)

install(DIRECTORY maps
  DESTINATION share/${PROJECT_NAME}
)

install(DIRECTORY models
  DESTINATION share/${PROJECT_NAME}
)

find_package(ament_cmake REQUIRED)
find_package(moveit_ros_planning_interface REQUIRED)
find_package(rclcpp REQUIRED)
find_package(moveit_visual_tools REQUIRED)
find_package(gazebo_msgs REQUIRED)
find_package(rviz_visual_tools REQUIRED)
find_package(geometry_msgs REQUIRED)

add_executable(frame_checker src/frame_checker.cpp)
ament_target_dependencies(frame_checker
  rclcpp
  moveit_ros_planning_interface
  "geometry_msgs"
  "gazebo_msgs"
  "rviz_visual_tools"
)

install(TARGETS
  frame_checker
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
