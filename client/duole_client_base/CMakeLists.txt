# path :
set(GAMEBASE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/gamebase)
set(EXTERNAL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/external)
set(SDKS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/sdks)
set(THIRD_SDKS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/third-sdks)

# gamebase
add_library(duole_gamebase STATIC IMPORTED GLOBAL)
set_target_properties(duole_gamebase PROPERTIES
  IMPORTED_LOCATION "${GAMEBASE_PATH}/prebuilt/android/${ANDROID_ABI}/libduole_gamebase.a"
)
# gamebase-include
set(GAMEBASE_INCLUDE_PATH)
list(APPEND GAMEBASE_INCLUDE_PATH "${GAMEBASE_PATH}/src")
set_target_properties(duole_gamebase PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${GAMEBASE_INCLUDE_PATH}"
)

# [external]
# protobuf
add_library(duole_protobuf STATIC IMPORTED GLOBAL)
set_target_properties(duole_protobuf PROPERTIES
  IMPORTED_LOCATION "${EXTERNAL_PATH}/google/prebuilt/android/${ANDROID_ABI}/libduole_protobuf.a"
)
# protobuf-include
set(PROTOBUF_INCLUDE_PATH)
list(APPEND PROTOBUF_INCLUDE_PATH "${EXTERNAL_PATH}/src/google")
set_target_properties(duole_protobuf PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${PROTOBUF_INCLUDE_PATH}"
)

# iconv
add_library(duole_iconv STATIC IMPORTED GLOBAL)
set_target_properties(duole_iconv PROPERTIES
  IMPORTED_LOCATION "${EXTERNAL_PATH}/iconv/prebuilt/android/${ANDROID_ABI}/libduole_iconv.a"
)
# iconv-include
set(ICONV_INCLUDE_PATH)
list(APPEND ICONV_INCLUDE_PATH "${EXTERNAL_PATH}/src/iconv")
set_target_properties(duole_iconv PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${ICONV_INCLUDE_PATH}"
)

# [third-sdks]
# bugly
add_library(duole_bugly STATIC IMPORTED GLOBAL)
set_target_properties(duole_bugly PROPERTIES
  IMPORTED_LOCATION "${THIRD_SDKS_PATH}/bugly/prebuilt/android/${ANDROID_ABI}/libduole_bugly.a"
)
# bugly-include
set(BUGLY_INCLUDE_PATH)
list(APPEND BUGLY_INCLUDE_PATH 
  "${THIRD_SDKS_PATH}/src"
  "${THIRD_SDKS_PATH}/src/bugly"
  "${THIRD_SDKS_PATH}/src/bugly/plugin"
  "${THIRD_SDKS_PATH}/src/bugly/plugin/lua"
)
set_target_properties(duole_bugly PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${BUGLY_INCLUDE_PATH}"
)

# bugly_v2
add_library(duole_bugly_v2 STATIC IMPORTED GLOBAL)
set_target_properties(duole_bugly_v2 PROPERTIES
  IMPORTED_LOCATION "${THIRD_SDKS_PATH}/bugly_v2/prebuilt/android/${ANDROID_ABI}/libduole_bugly_v2.a"
)
# bugly_v2-include
set(BUGLY_INCLUDE_PATH)
list(APPEND BUGLY_INCLUDE_PATH 
  "${THIRD_SDKS_PATH}/src"
  "${THIRD_SDKS_PATH}/src/bugly_v2"
  "${THIRD_SDKS_PATH}/src/bugly_v2/plugin"
  "${THIRD_SDKS_PATH}/src/bugly_v2/plugin/lua"
)
set_target_properties(duole_bugly_v2 PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${BUGLY_INCLUDE_PATH}"
)

# [sdks]
# platform
add_library(duole_sdks_platform STATIC IMPORTED GLOBAL)
set_target_properties(duole_sdks_platform PROPERTIES
  IMPORTED_LOCATION "${SDKS_PATH}/platform/prebuilt/android/${ANDROID_ABI}/libduole_sdks_platform.a"
)
# platform-include
set(PLATFORM_INCLUDE_PATH)
list(APPEND PLATFORM_INCLUDE_PATH 
  "${SDKS_PATH}/src/platform"
  "${SDKS_PATH}/src"
)
set_target_properties(duole_sdks_platform PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${PLATFORM_INCLUDE_PATH}"
)


