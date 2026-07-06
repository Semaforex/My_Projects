cmake_minimum_required(VERSION 3.5)
project(nav2_tilt_costmap_plugin)

set(lib_name ${PROJECT_NAME}_core)

# === Environment ===

if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# === Dependencies ===

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(nav2_costmap_2d REQUIRED)
find_package(pluginlib REQUIRED)
find_package(nvblox_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)
find_package(tf2_sensor_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)



# === Build ===

add_library(${lib_name} SHARED
            src/tilt_layer.cpp)
include_directories(include)

# === Installation ===

install(TARGETS ${lib_name}
        DESTINATION lib)

# === Ament work ===

pluginlib_export_plugin_description_file(nav2_costmap_2d tilt_layer.xml)

# USUNIĘTE SŁOWO "PUBLIC" - teraz wszystko jest "plain signature"
target_link_libraries(${lib_name}
  nav2_costmap_2d::filters
  nav2_costmap_2d::layers
  nav2_costmap_2d::nav2_costmap_2d_client
  nav2_costmap_2d::nav2_costmap_2d_core
  pluginlib::pluginlib
  rclcpp::rclcpp
)

ament_target_dependencies(${lib_name}
  nvblox_msgs
  tf2
  tf2_ros
  tf2_geometry_msgs
  tf2_sensor_msgs
  sensor_msgs
)

ament_package()