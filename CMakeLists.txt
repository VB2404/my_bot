cmake_minimum_required(VERSION 3.5)
project(my_bot)

# Standards 
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

add_compile_options(-Wall -Wextra -Wpedantic)

# Dependencies 
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(hardware_interface REQUIRED)
find_package(pluginlib REQUIRED)
find_package(std_msgs REQUIRED)

# Library 
add_library(my_bot_hardware SHARED
  hardware/dvl_hardware.cpp
)

target_include_directories(my_bot_hardware PUBLIC
  include
)

ament_target_dependencies(my_bot_hardware
  rclcpp
  hardware_interface
  pluginlib
  std_msgs
)

# Plugin export
pluginlib_export_plugin_description_file(
  hardware_interface
  my_bot_hardware.xml
)

# Install library
install(
  TARGETS my_bot_hardware
  LIBRARY DESTINATION lib
)

# Install headers
install(
  DIRECTORY include/
  DESTINATION include/
)

# Install plugin XML (IMPORTANT)
install(
  FILES my_bot_hardware.xml
  DESTINATION share/${PROJECT_NAME}
)

# Install other resources
install(
  DIRECTORY config description launch worlds
  DESTINATION share/${PROJECT_NAME}
)

ament_package()