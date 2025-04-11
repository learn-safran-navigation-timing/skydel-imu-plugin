set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(TARGET_NAME imu_common)

find_package(
  Qt5
  COMPONENTS Network SerialPort
  REQUIRED)

file(GLOB_RECURSE SOURCE_FILES *.cpp *.ui *.h)

add_library(${TARGET_NAME} STATIC ${SOURCE_FILES})

target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(
  ${TARGET_NAME} PUBLIC inertial_math SkydelPlugin Qt5::Core Qt5::Widgets
                        Qt5::Network Qt5::SerialPort)
