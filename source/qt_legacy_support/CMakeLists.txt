find_package(
    Qt5
    COMPONENTS SerialPort
    REQUIRED
)

file(GLOB QT_LEGACY_SUPPORT_SRC *.cpp)

add_library(qt_legacy_support STATIC ${QT_LEGACY_SUPPORT_SRC})

target_include_directories(qt_legacy_support PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(qt_legacy_support PRIVATE Qt5::Core Qt5::SerialPort)

if(WIN32)
    add_compile_definitions(UNICODE)
endif(WIN32)
