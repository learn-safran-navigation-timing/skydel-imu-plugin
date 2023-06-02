set(blaze_ROOT ${CMAKE_SOURCE_DIR}/external/blaze)
set(blaze_VERSION 3.7)

find_path(blaze_INCLUDE_DIRS
    NAMES blaze/Math.h
    HINTS ${blaze_ROOT}/include
    )

if(WIN32)
    if (CMAKE_BUILD_TYPE MATCHES Release)
      set(blaze_LIBRARIES
        ${blaze_ROOT}/build/win64/lapack.lib
        ${blaze_ROOT}/build/win64/blas.lib
        ${blaze_ROOT}/build/win64/libf2c.lib
      )
    elseif(CMAKE_BUILD_TYPE MATCHES Debug)
      set(blaze_LIBRARIES
            ${blaze_ROOT}/build/win64/lapackd.lib
            ${blaze_ROOT}/build/win64/blasd.lib
            ${blaze_ROOT}/build/win64/libf2cd.lib
      )
    endif()
else()
    set(blaze_LIBRARIES lapack)
endif(WIN32)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(blaze DEFAULT_MSG
  blaze_INCLUDE_DIRS
  blaze_LIBRARIES
  )

add_library(blaze INTERFACE)

target_include_directories(blaze SYSTEM INTERFACE ${blaze_INCLUDE_DIRS})
target_link_libraries(blaze INTERFACE ${blaze_LIBRARIES})

mark_as_advanced(blaze_INCLUDE_DIRS)
mark_as_advanced(blaze_LIBRARIES)
