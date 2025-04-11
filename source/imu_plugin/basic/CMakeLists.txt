set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(
  Qt5
  COMPONENTS Network
  REQUIRED)

file(GLOB_RECURSE SOURCE_FILES *.cpp *.ui *.h)

add_library(imu_basic_plugin SHARED ${SOURCE_FILES})

target_link_libraries(imu_basic_plugin PUBLIC SkydelPlugin Qt5::Network
                                              imu_common inertial_math)

target_include_directories(imu_basic_plugin PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(imu_basic_plugin PROPERTIES CXX_STANDARD 20)

target_compile_definitions(
  imu_basic_plugin PRIVATE PLUGIN_IID="imu_basic_plugin"
                           PLUGIN_FILE="imu_basic_plugin.json")

install(TARGETS imu_basic_plugin RUNTIME DESTINATION ${PLUGIN_INSTALL_DIR})
