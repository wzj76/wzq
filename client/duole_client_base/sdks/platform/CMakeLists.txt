cmake_minimum_required(VERSION 3.6)

set(lib_name sdks_platform)
set(target_name duole_${lib_name})
project(${lib_name})

add_library(${target_name} STATIC IMPORTED GLOBAL)

set_target_properties(${target_name} PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/../src/platform"
)

set(_path_prefix ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/)
set(platform_spec_path android/${ANDROID_ABI})
set(platform_spec_path "${_path_prefix}${platform_spec_path}")

set_target_properties(${target_name} PROPERTIES
  IMPORTED_LOCATION "${platform_spec_path}/lib${target_name}.a"
)
