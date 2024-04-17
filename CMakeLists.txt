cmake_minimum_required(VERSION 3.17)

project(SkydelImuPlugin VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

if(NOT WIN32)
    set(CMAKE_CXX_COMPILER /usr/bin/g++-11 CACHE FILEPATH "GNU C++ compiler")
    set(Qt5_VERSION 5.15.2 CACHE STRING "Version of Qt5 you want to find")
    set(Qt5_ROOT /opt/Qt CACHE FILEPATH "Directory where you installed Qt5")
    set(Qt5_DIR "${Qt5_ROOT}/${Qt5_VERSION}/gcc_64/lib/cmake" CACHE FILEPATH "")
    list(APPEND CMAKE_PREFIX_PATH ${Qt5_DIR})
    set(PLUGIN_INSTALL_DIR "$ENV{HOME}/Documents/Skydel-SDX/Plug-ins" CACHE PATH "Define the destination folder where the plug-in examples are installed")
else()
    list(APPEND CMAKE_PREFIX_PATH "C:/Qt/5.15.2/msvc2019_64/lib/cmake")
    set(PLUGIN_INSTALL_DIR "$ENV{HOMEPATH}/Documents/Skydel-SDX/Plug-ins" CACHE PATH "Define the destination folder where the plug-in examples are installed")
endif()

# Dependencies
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)
find_package(blaze 3.7 REQUIRED)
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

# Skydel Plugins SDK
add_library(SkydelPlugin INTERFACE)
target_include_directories(SkydelPlugin INTERFACE include )
target_link_libraries(SkydelPlugin INTERFACE Qt5::Core Qt5::Widgets)

# IMU Plugin
add_subdirectory(source)
