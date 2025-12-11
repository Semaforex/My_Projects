cmake_minimum_required(VERSION 3.8)
project(nav_lab1)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)

# Definicja pliku wykonywalnego
add_executable(piaciokotforemny src/piaciokotforemny.cpp)
add_executable(odometry_info src/odometry_info.cpp)

# Linkowanie zależności
ament_target_dependencies(piaciokotforemny 
  rclcpp 
  nav_msgs
  geometry_msgs
)
ament_target_dependencies(odometry_info
  rclcpp
  nav_msgs
)

# Instalacja (aby 'ros2 run' widział węzeł)
install(TARGETS
  piaciokotforemny
  odometry_info
  DESTINATION lib/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()