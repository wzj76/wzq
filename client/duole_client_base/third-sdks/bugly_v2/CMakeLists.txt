cmake_minimum_required(VERSION 3.6)

set(lib_name bugly_v2)
set(target_name duole_${lib_name})
project(${lib_name})

add_library(${target_name} STATIC IMPORTED GLOBAL)

list(APPEND BUGLY_HEADER
  "${CMAKE_CURRENT_SOURCE_DIR}/../src"
  "${CMAKE_CURRENT_SOURCE_DIR}/../src/${lib_name}"
  "${CMAKE_CURRENT_SOURCE_DIR}/../src/${lib_name}/plugin"
  "${CMAKE_CURRENT_SOURCE_DIR}/../src/${lib_name}/plugin/lua"
)
set_target_properties(${target_name} PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${BUGLY_HEADER}"
)

set(_path_prefix ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/)
set(platform_spec_path android/${ANDROID_ABI})
set(platform_spec_path "${_path_prefix}${platform_spec_path}")

set_target_properties(${target_name} PROPERTIES
  IMPORTED_LOCATION "${platform_spec_path}/lib${target_name}.a"
)
