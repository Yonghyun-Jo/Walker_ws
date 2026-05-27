cmake_minimum_required(VERSION 3.8)
project(mjc_ros2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic -Wno-missing-field-initializers)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(p73_lib REQUIRED)
find_package(br_driver REQUIRED)

# Set the MuJoCo path
set(MUJOCO_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../mujoco-3.3.7)
message("MuJoCo path: ${MUJOCO_PATH}")

include_directories(
  include
  ${MUJOCO_PATH}/include
  ${MUJOCO_PATH}/simulate
  ${p73_lib_INCLUDE_DIRS}
  /usr/local/include/br_driver
)

link_directories(
  ${MUJOCO_PATH}/lib
)

add_executable(mujoco_ros2
  src/main.cpp
  ${MUJOCO_PATH}/simulate/simulate.cc
  ${MUJOCO_PATH}/simulate/glfw_adapter.cc
  ${MUJOCO_PATH}/simulate/glfw_dispatch.cc
  ${MUJOCO_PATH}/simulate/platform_ui_adapter.cc
  ${MUJOCO_PATH}/simulate/lodepng.cpp
)

# Find GLFW package
find_package(glfw3 REQUIRED)

# Set RPATH to find MuJoCo library
set_target_properties(mujoco_ros2 PROPERTIES
  INSTALL_RPATH "${MUJOCO_PATH}/lib"
  BUILD_WITH_INSTALL_RPATH TRUE
)

target_link_libraries(mujoco_ros2
  ${MUJOCO_PATH}/lib/libmujoco.so.3.3.7
  br_driver
  GL
  glfw
)

ament_target_dependencies(mujoco_ros2
  rclcpp
  std_msgs
  sensor_msgs
)

# Headless executable (no GLFW, no GUI — for automated evaluation)
add_executable(mujoco_ros2_headless
  src/main_headless.cpp
)

set_target_properties(mujoco_ros2_headless PROPERTIES
  INSTALL_RPATH "${MUJOCO_PATH}/lib"
  BUILD_WITH_INSTALL_RPATH TRUE
)

target_link_libraries(mujoco_ros2_headless
  ${MUJOCO_PATH}/lib/libmujoco.so.3.3.7
  br_driver
)

ament_target_dependencies(mujoco_ros2_headless
  rclcpp
  std_msgs
  sensor_msgs
)

install(TARGETS
  mujoco_ros2
  mujoco_ros2_headless
  DESTINATION lib/${PROJECT_NAME}
)

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
