file(GLOB QT_LEGACY_SUPPORT_SRC *.cpp)

add_library(qt_legacy_support STATIC ${QT_LEGACY_SUPPORT_SRC})

target_include_directories(qt_legacy_support PUBLIC ${SDX_PLUGIN_LIBRARIES_DIR})

target_link_libraries(qt_legacy_support PRIVATE Qt5::Core Qt5::SerialPort)
