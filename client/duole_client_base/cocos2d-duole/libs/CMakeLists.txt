
# path :
set(LIBS_LOCAL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/android/${ANDROID_ABI}/lib)
set(LIBS_EXTERNAL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../external)

# cocos2d 
# cocos2d-lib
add_library(cocos2d STATIC IMPORTED GLOBAL)
set_target_properties(cocos2d PROPERTIES
  IMPORTED_LOCATION "${LIBS_LOCAL_PATH}/libcocos2d.a"
)
# cocos2d-depend
set(COCOS2D_DEPEND_LIBS)
list(APPEND COCOS2D_DEPEND_LIBS "ext_websockets")
list(APPEND COCOS2D_DEPEND_LIBS "ext_linearmath")
list(APPEND COCOS2D_DEPEND_LIBS "ext_bulletcollision")
list(APPEND COCOS2D_DEPEND_LIBS "ext_bulletdynamics")
list(APPEND COCOS2D_DEPEND_LIBS "ext_bulletmultithreaded")
list(APPEND COCOS2D_DEPEND_LIBS "ext_minicl")
set_property(TARGET cocos2d APPEND PROPERTY INTERFACE_LINK_LIBRARIES ${COCOS2D_DEPEND_LIBS})

# cocos2d-include
set(COCOS2D_INCLUDE_PATH)
list(APPEND COCOS2D_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/..")
list(APPEND COCOS2D_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cocos")
list(APPEND COCOS2D_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cocos/audio/include")
list(APPEND COCOS2D_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cocos/platform/android")
list(APPEND COCOS2D_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cocos/editor-support")
list(APPEND COCOS2D_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cocos/scripting/lua-bindings/auto")
list(APPEND COCOS2D_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cocos/scripting/lua-bindings/manual")
list(APPEND COCOS2D_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../extensions")
list(APPEND COCOS2D_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../external")
list(APPEND COCOS2D_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../external/lua")

set_target_properties(cocos2d PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${COCOS2D_INCLUDE_PATH}"
)

# luacocos2d
add_library(luacocos2d STATIC IMPORTED GLOBAL)
set_target_properties(luacocos2d PROPERTIES
  IMPORTED_LOCATION "${LIBS_LOCAL_PATH}/libluacocos2d.a"
)
set(LUACOCOS2D_DEPEND_LIBS)
list(APPEND LUACOCOS2D_DEPEND_LIBS "cocos2d")
list(APPEND LUACOCOS2D_DEPEND_LIBS "ext_lua515")
list(APPEND LUACOCOS2D_DEPEND_LIBS "ext_tolua")
list(APPEND LUACOCOS2D_DEPEND_LIBS "ext_luasocket")
list(APPEND LUACOCOS2D_DEPEND_LIBS "ext_cjson")
list(APPEND LUACOCOS2D_DEPEND_LIBS "ext_lua_extensions")
set_property(TARGET luacocos2d APPEND PROPERTY INTERFACE_LINK_LIBRARIES ${LUACOCOS2D_DEPEND_LIBS})

# cpp_android_spec
add_library(cpp_android_spec STATIC IMPORTED GLOBAL)
set_target_properties(cpp_android_spec PROPERTIES
  IMPORTED_LOCATION "${LIBS_LOCAL_PATH}/libcpp_android_spec.a"
)
# lua_android_spec
add_library(lua_android_spec STATIC IMPORTED GLOBAL)
set_target_properties(lua_android_spec PROPERTIES
  IMPORTED_LOCATION "${LIBS_LOCAL_PATH}/liblua_android_spec.a"
)

# cocos2d-external
# src :
list(APPEND COCOS2D_EXT_LIBS "ext_cjson")
list(APPEND COCOS2D_EXT_LIBS "ext_clipper")
list(APPEND COCOS2D_EXT_LIBS "ext_convertUTF")
list(APPEND COCOS2D_EXT_LIBS "ext_edtaa3func")
list(APPEND COCOS2D_EXT_LIBS "ext_lua_extensions")
list(APPEND COCOS2D_EXT_LIBS "ext_lua515")
list(APPEND COCOS2D_EXT_LIBS "ext_luasocket")
list(APPEND COCOS2D_EXT_LIBS "ext_md5")
list(APPEND COCOS2D_EXT_LIBS "ext_poly2tri")
list(APPEND COCOS2D_EXT_LIBS "ext_pvmp3dec")
list(APPEND COCOS2D_EXT_LIBS "ext_cpufeatures")
list(APPEND COCOS2D_EXT_LIBS "ext_recast")
list(APPEND COCOS2D_EXT_LIBS "ext_tinyxml2")
list(APPEND COCOS2D_EXT_LIBS "ext_tolua")
list(APPEND COCOS2D_EXT_LIBS "ext_tremolo")
list(APPEND COCOS2D_EXT_LIBS "ext_unzip")
list(APPEND COCOS2D_EXT_LIBS "ext_xxhash")
list(APPEND COCOS2D_EXT_LIBS "ext_xxtea")

foreach(ext_lib ${COCOS2D_EXT_LIBS})
  add_library(${ext_lib} STATIC IMPORTED GLOBAL)
  set_target_properties(${ext_lib} PROPERTIES
    IMPORTED_LOCATION "${LIBS_LOCAL_PATH}/lib${ext_lib}.a"
  )
endforeach(ext_lib)

# include
set_target_properties(ext_lua515 PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/../external/lua"
)
set_target_properties(ext_lua515 PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/../external/lua/lua"
)
set_target_properties(ext_tolua PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/../external/lua/tolua"
)

# cocos2d-external-prebuilt
# chipmunk
add_library(ext_chipmunk STATIC IMPORTED GLOBAL)
set_target_properties(ext_chipmunk PROPERTIES
  IMPORTED_LOCATION "${LIBS_EXTERNAL_PATH}/chipmunk/prebuilt/android/${ANDROID_ABI}/libchipmunk.a"
)
set_target_properties(ext_chipmunk PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${LIBS_EXTERNAL_PATH}/chipmunk/include"
)

# Box2D
add_library(ext_box2d STATIC IMPORTED GLOBAL)
set_target_properties(ext_box2d PROPERTIES
  IMPORTED_LOCATION "${LIBS_EXTERNAL_PATH}/Box2D/prebuilt/android/${ANDROID_ABI}/libbox2d.a"
)
set_target_properties(ext_box2d PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${LIBS_EXTERNAL_PATH}/Box2D/include"
)

# freetype
add_library(ext_freetype STATIC IMPORTED GLOBAL)
set_target_properties(ext_freetype PROPERTIES
  IMPORTED_LOCATION "${LIBS_EXTERNAL_PATH}/freetype2/prebuilt/android/${ANDROID_ABI}/libfreetype.a"
)
set_target_properties(ext_freetype PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${LIBS_EXTERNAL_PATH}/freetype2/include/android/freetype2"
)

# jpeg
add_library(ext_jpeg STATIC IMPORTED GLOBAL)
set_target_properties(ext_jpeg PROPERTIES
  IMPORTED_LOCATION "${LIBS_EXTERNAL_PATH}/jpeg/prebuilt/android/${ANDROID_ABI}/libjpeg.a"
)
set_target_properties(ext_jpeg PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${LIBS_EXTERNAL_PATH}/jpeg/include/android"
)

# uv
add_library(ext_uv STATIC IMPORTED GLOBAL)
set_target_properties(ext_uv PROPERTIES
  IMPORTED_LOCATION "${LIBS_EXTERNAL_PATH}/uv/prebuilt/android/${ANDROID_ABI}/libuv_a.a"
)
set_target_properties(ext_uv PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${LIBS_EXTERNAL_PATH}/uv/include"
)

# webp
add_library(ext_webp STATIC IMPORTED GLOBAL)
set_target_properties(ext_webp PROPERTIES
  IMPORTED_LOCATION "${LIBS_EXTERNAL_PATH}/webp/prebuilt/android/${ANDROID_ABI}/libwebp.a"
)
set_target_properties(ext_webp PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${LIBS_EXTERNAL_PATH}/webp/include/android"
)
set_property(TARGET ext_webp APPEND PROPERTY INTERFACE_LINK_LIBRARIES "ext_cpufeatures")

# websockets
add_library(ext_websockets STATIC IMPORTED GLOBAL)
set_target_properties(ext_websockets PROPERTIES
  IMPORTED_LOCATION "${LIBS_EXTERNAL_PATH}/websockets/prebuilt/android/${ANDROID_ABI}/libwebsockets.a"
)
set_target_properties(ext_websockets PROPERTIES
  INTERFACE_COMPILE_DEFINITIONS "LWS_WITH_LIBUV"
)
set_property(TARGET ext_websockets APPEND PROPERTY INTERFACE_LINK_LIBRARIES "ext_ssl")
set_property(TARGET ext_websockets APPEND PROPERTY INTERFACE_LINK_LIBRARIES "ext_crypto")
set_property(TARGET ext_websockets APPEND PROPERTY INTERFACE_LINK_LIBRARIES "ext_uv")
set_target_properties(ext_websockets PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${LIBS_EXTERNAL_PATH}/websockets/include/android"
)

# ssl/crypto
add_library(ext_ssl STATIC IMPORTED GLOBAL)
set_target_properties(ext_ssl PROPERTIES
  IMPORTED_LOCATION "${LIBS_EXTERNAL_PATH}/openssl/prebuilt/android/${ANDROID_ABI}/libssl.a"
)
set_target_properties(ext_ssl PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${LIBS_EXTERNAL_PATH}/openssl/include/android"
)

add_library(ext_crypto STATIC IMPORTED GLOBAL)
set_target_properties(ext_crypto PROPERTIES
  IMPORTED_LOCATION "${LIBS_EXTERNAL_PATH}/openssl/prebuilt/android/${ANDROID_ABI}/libcrypto.a"
)
set_target_properties(ext_crypto PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${LIBS_EXTERNAL_PATH}/openssl/include/android"
)

# tiff
add_library(ext_tiff STATIC IMPORTED GLOBAL)
set_target_properties(ext_tiff PROPERTIES
  IMPORTED_LOCATION "${LIBS_EXTERNAL_PATH}/tiff/prebuilt/android/${ANDROID_ABI}/libtiff.a"
)
set_target_properties(ext_tiff PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${LIBS_EXTERNAL_PATH}/tiff/include/android"
)

# bullet-libs
add_library(ext_linearmath STATIC IMPORTED GLOBAL)
set_target_properties(ext_linearmath PROPERTIES
  IMPORTED_LOCATION "${LIBS_EXTERNAL_PATH}/bullet/prebuilt/android/${ANDROID_ABI}/libLinearMath.a"
)
add_library(ext_bulletcollision STATIC IMPORTED GLOBAL)
set_target_properties(ext_bulletcollision PROPERTIES
  IMPORTED_LOCATION "${LIBS_EXTERNAL_PATH}/bullet/prebuilt/android/${ANDROID_ABI}/libBulletCollision.a"
)
add_library(ext_bulletdynamics STATIC IMPORTED GLOBAL)
set_target_properties(ext_bulletdynamics PROPERTIES
  IMPORTED_LOCATION "${LIBS_EXTERNAL_PATH}/bullet/prebuilt/android/${ANDROID_ABI}/libBulletDynamics.a"
)
add_library(ext_bulletmultithreaded STATIC IMPORTED GLOBAL)
set_target_properties(ext_bulletmultithreaded PROPERTIES
  IMPORTED_LOCATION "${LIBS_EXTERNAL_PATH}/bullet/prebuilt/android/${ANDROID_ABI}/libBulletMultiThreaded.a"
)
add_library(ext_minicl STATIC IMPORTED GLOBAL)
set_target_properties(ext_minicl PROPERTIES
  IMPORTED_LOCATION "${LIBS_EXTERNAL_PATH}/bullet/prebuilt/android/${ANDROID_ABI}/libMiniCL.a"
)
set_property(TARGET ext_bulletcollision APPEND PROPERTY INTERFACE_LINK_LIBRARIES "ext_linearmath")
set_property(TARGET ext_bulletdynamics APPEND PROPERTY INTERFACE_LINK_LIBRARIES "ext_bulletcollision")

set_target_properties(ext_linearmath PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${LIBS_EXTERNAL_PATH}/bullet/include"
)
set_target_properties(ext_linearmath PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${LIBS_EXTERNAL_PATH}/bullet/include/bullet"
)

# curl
add_library(ext_curl STATIC IMPORTED GLOBAL)
set_target_properties(ext_curl PROPERTIES
  IMPORTED_LOCATION "${LIBS_EXTERNAL_PATH}/curl/prebuilt/android/${ANDROID_ABI}/libcurl.a"
)
set_target_properties(ext_curl PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${LIBS_EXTERNAL_PATH}/curl/include/android"
)

# png
add_library(ext_png STATIC IMPORTED GLOBAL)
set_target_properties(ext_png PROPERTIES
  IMPORTED_LOCATION "${LIBS_EXTERNAL_PATH}/png/prebuilt/android/${ANDROID_ABI}/libpng.a"
)
set_target_properties(ext_png PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${LIBS_EXTERNAL_PATH}/png/include/android"
)

# zlib
add_library(ext_zlib STATIC IMPORTED GLOBAL)
set_target_properties(ext_zlib PROPERTIES
  IMPORTED_LOCATION "${LIBS_EXTERNAL_PATH}/zlib/prebuilt/android/${ANDROID_ABI}/libz.a"
)
set_target_properties(ext_zlib PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${LIBS_EXTERNAL_PATH}/zlib/include"
)

