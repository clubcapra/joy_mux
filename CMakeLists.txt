cmake_minimum_required(VERSION 3.8)
project(joy_mux)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
#TODO : ajouter les dependances necessaires (pas sur de comment faire)
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)

set( DEPENDENCIES
  "rclcpp"
  "std_msgs"
  "sensor_msgs"
)

include_directories(include)

add_executable(joy_mux 
  src/joy_mux.cpp
  src/joy_mux_node.cpp
)
ament_target_dependencies(joy_mux ${DEPENDENCIES})

install(
  DIRECTORY include/
  DESTINATION include
)

#Install Node
install(
  TARGETS joy_mux
  # DESTINATION lib/${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION lib/${PROJECT_NAME}
)

foreach(dir launch config)
  install(DIRECTORY ${dir}/
    DESTINATION share/${PROJECT_NAME}/${dir})
endforeach()

# if(BUILD_TESTING)
#   find_package(ament_lint_auto REQUIRED)
#   # the following line skips the linter which checks for copyrights
#   # comment the line when a copyright and license is added to all source files
#   set(ament_cmake_copyright_FOUND TRUE)
#   # the following line skips cpplint (only works in a git repo)
#   # comment the line when this package is in a git repo and when
#   # a copyright and license is added to all source files
#   set(ament_cmake_cpplint_FOUND TRUE)
#   ament_lint_auto_find_test_dependencies()
# endif()

ament_export_include_directories(include)#pas sur si necessaire
ament_export_dependencies(${DEPENDENCIES}) 

ament_package()
