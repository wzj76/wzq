# log
message(STATUS "PROJECT_NAME:" ${PROJECT_NAME})
message(STATUS "PROJECT_SOURCE_DIR:" ${PROJECT_SOURCE_DIR})
message(STATUS "CMAKE_MODULE_PATH:" ${CMAKE_MODULE_PATH})
message(STATUS "DUOLE_ROOT_PATH:" ${DUOLE_ROOT_PATH})

# include helper functions
include(BuildHelpers)

# set common compiler options
# add target compile define function
# add target compile options function
include(BuildDefine)

# config libraries dependence
include(Dependencies)
