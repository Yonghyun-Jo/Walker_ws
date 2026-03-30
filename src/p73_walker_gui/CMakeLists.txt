cmake_minimum_required(VERSION 3.8)
project(p73_gui)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(p73_msgs REQUIRED)

find_package(rqt_gui_cpp REQUIRED)
find_package(qt_gui_cpp REQUIRED)
find_package(pluginlib REQUIRED)

# Qt5
find_package(Qt5 REQUIRED COMPONENTS Widgets Core Gui)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)        # .ui 자동 처리 (Qt>=5.9)
set(CMAKE_AUTORCC ON)        # .qrc 자동 처리

# UI 파일 경로를 명시적으로 설정
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/resource)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 리소스/아이콘이 필요 없으면 icons.qrc는 빼도 됨
set(UI_FILES
  resource/p73.ui
)
set(QRC_FILES
  resource/icon.qrc
)

include_directories(
  include
)

add_library(p73_gui_plugin SHARED
  src/p73_gui.cpp
  src/window.cpp
  include/p73_gui/p73_gui.hpp
  include/p73_gui/window.hpp
  ${UI_FILES}
  ${QRC_FILES}
)

ament_target_dependencies(p73_gui_plugin
  rclcpp
  rqt_gui_cpp
  qt_gui_cpp
  pluginlib
  p73_msgs
  std_msgs
  sensor_msgs
)

target_link_libraries(p73_gui_plugin Qt5::Widgets)
pluginlib_export_plugin_description_file(rqt_gui plugin.xml)
# 플러그인 설치
install(TARGETS p73_gui_plugin
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)
install(DIRECTORY resource/
  DESTINATION share/${PROJECT_NAME}/resource
  FILES_MATCHING PATTERN "*.ui" PATTERN "*.jpeg" PATTERN "*.qrc" PATTERN "*.png")
install(FILES plugin.xml
  DESTINATION share/${PROJECT_NAME}
)
install(PROGRAMS scripts/p73_gui
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
