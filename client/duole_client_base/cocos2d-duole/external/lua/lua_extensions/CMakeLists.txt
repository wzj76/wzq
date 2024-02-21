cmake_minimum_required(VERSION 3.6)

set(lib_name lua_extensions)
set(target_name ext_${lib_name})

project(${lib_name})

set(${target_name}_src
  ../lpack/lpack.c
  ../lpack/lua_pack_ext.c
  ../lsqlite3/lsqlite3.c
  ../lsqlite3/sqlite3.c
  ../lsqlite3/lua_sqlite3_ext.c
  ../luaprotobuf/pb.c
  ../luaprotobuf/lua_pb_ext.c
  ../zlib/lua_zlib.c
  ../zlib/lua_zlib_ext.c
)

add_library(${target_name} STATIC
  ${${target_name}_src}
)

target_include_directories(${target_name} 
  PRIVATE ${COCOS2DX_ROOT_PATH}/cocos
  PRIVATE ../lua
  PRIVATE ../lpack
  PRIVATE ../lsqlite3
  PRIVATE ../luaprotobuf
  PRIVATE ../zlib
  )
target_include_directories(${target_name} PUBLIC .)

set_target_properties(${target_name}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    FOLDER "External"
)
