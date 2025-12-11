cmake_minimum_required(VERSION 3.8)
project(pick_place)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(moveit_ros_planning_interface REQUIRED)
find_package(rclcpp REQUIRED)
find_package(moveit_visual_tools REQUIRED)
find_package(gazebo_msgs REQUIRED)
find_package(rviz_visual_tools REQUIRED)
find_package(geometry_msgs REQUIRED)


add_executable(pick_place src/pick_place.cpp)
add_executable(pick_place_2 src/pick_place_2.cpp)

add_executable(pick_place_vis src/pick_place_vis.cpp)
target_include_directories(pick_place PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_compile_features(pick_place PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++17
# Add rclcpp and gazebo_msgs to pick_place target
ament_target_dependencies(
  pick_place
  "moveit_ros_planning_interface"
  "moveit_visual_tools"
  "rclcpp"
  "geometry_msgs"
  "gazebo_msgs"
  "rviz_visual_tools"
)
ament_target_dependencies(
  pick_place_2
  "moveit_ros_planning_interface"
  "moveit_visual_tools"
  "rclcpp"
  "geometry_msgs"
  "gazebo_msgs"
  "rviz_visual_tools"
)




ament_target_dependencies(
  pick_place_vis
  "moveit_ros_planning_interface"
  "moveit_visual_tools"
  "rclcpp"
  "geometry_msgs"
  "gazebo_msgs"
  "rviz_visual_tools"
)


install(TARGETS pick_place
  DESTINATION lib/${PROJECT_NAME})
install(TARGETS pick_place_vis
  DESTINATION lib/${PROJECT_NAME})
install(TARGETS pick_place_2
  DESTINATION lib/${PROJECT_NAME})


if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
