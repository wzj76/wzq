cmake_minimum_required(VERSION 3.6)

set(lib_name iconv)
set(target_name duole_${lib_name})

project(${lib_name})

set(${target_name}_src 
    libcharset/lib/localcharset.c 
    lib/iconv.c 
    # lib/relocatable.c
)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-multichar -DANDROID \
    -DLIBDIR=\"c\" -DBUILDING_LIBICONV -DIN_LIBRARY ")

add_library(${target_name} STATIC
  ${${target_name}_src}
)

target_include_directories(${target_name}
    PUBLIC .
    PRIVATE include
    PRIVATE libcharset
    PRIVATE lib
    PRIVATE libcharset/include
    PRIVATE srclib
)

set(_path_prefix ${CMAKE_CURRENT_SOURCE_DIR}/../../iconv/prebuilt/)
set(platform_name android)
set(platform_spec_path android/${ANDROID_ABI})
set(platform_spec_path "${_path_prefix}${platform_spec_path}")

set_target_properties(${target_name}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${platform_spec_path}"
)
