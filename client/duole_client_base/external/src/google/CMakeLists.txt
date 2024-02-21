cmake_minimum_required(VERSION 3.6)

set(lib_name protobuf)
set(target_name duole_${lib_name})
project(${lib_name})

set(${target_name}_src 
    google/protobuf/descriptor.pb.cc 
    google/protobuf/descriptor.cc 
    google/protobuf/descriptor_database.cc 
    google/protobuf/dynamic_message.cc 
    google/protobuf/extension_set.cc 
    google/protobuf/extension_set_heavy.cc 
    google/protobuf/generated_message_reflection.cc 
    google/protobuf/generated_message_util.cc 
    google/protobuf/message.cc 
    google/protobuf/message_lite.cc 
    google/protobuf/reflection_ops.cc 
    google/protobuf/repeated_field.cc 
    google/protobuf/service.cc 
    google/protobuf/text_format.cc 
    google/protobuf/unknown_field_set.cc 
    google/protobuf/wire_format.cc 
    google/protobuf/wire_format_lite.cc 
    
    google/protobuf/io/coded_stream.cc 
#     google/protobuf/io/gzip_stream.cc 
    google/protobuf/io/printer.cc 
    google/protobuf/io/tokenizer.cc 
    google/protobuf/io/zero_copy_stream.cc 
    google/protobuf/io/zero_copy_stream_impl.cc 
    google/protobuf/io/zero_copy_stream_impl_lite.cc 
    
    google/protobuf/stubs/common.cc 
#     google/protobuf/stubs/once.cc 
    google/protobuf/stubs/structurally_valid.cc 
    google/protobuf/stubs/strutil.cc 
    google/protobuf/stubs/substitute.cc
)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DANDROID \
    -fexceptions -fstrict-aliasing -g -fvisibility-inlines-hidden \
    -std=c++11 -D__GXX_EXPERIMENTAL_CXX0X__ ")

add_library(${target_name} STATIC
  ${${target_name}_src}
)

target_include_directories(${target_name}
    PUBLIC .
    PRIVATE google/protobuf/
    PRIVATE google/protobuf/io
    PRIVATE google/protobuf/stubs
)

set(_path_prefix ${CMAKE_CURRENT_SOURCE_DIR}/../../google/prebuilt/)
set(platform_name android)
set(platform_spec_path android/${ANDROID_ABI})
set(platform_spec_path "${_path_prefix}${platform_spec_path}")

set_target_properties(${target_name}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${platform_spec_path}"
)
