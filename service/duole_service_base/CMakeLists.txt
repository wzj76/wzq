# path :
set(SERVICEBASE_PATH ${DUOLE_ROOT_PATH}/servicebase)
set(EXTERNAL_PATH ${DUOLE_ROOT_PATH}/external)

# servicebase
add_library(duole_servicebase STATIC IMPORTED GLOBAL)
set_target_properties(duole_servicebase PROPERTIES
  IMPORTED_LOCATION "${SERVICEBASE_PATH}/prebuilt/${SYSTEM_OS}/libservice_base.a"
)
# servicebase-include
set(SERVICEBASE_INCLUDE_PATH)
list(APPEND SERVICEBASE_INCLUDE_PATH "${SERVICEBASE_PATH}/src")
list(APPEND SERVICEBASE_INCLUDE_PATH "${SERVICEBASE_PATH}/src/mcgcommon")
list(APPEND SERVICEBASE_INCLUDE_PATH "${SERVICEBASE_PATH}/src/mcgservice")
list(APPEND SERVICEBASE_INCLUDE_PATH "${SERVICEBASE_PATH}/src/mcgutil")
list(APPEND SERVICEBASE_INCLUDE_PATH "${SERVICEBASE_PATH}/src/mcghttp")

set_target_properties(duole_servicebase PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${SERVICEBASE_INCLUDE_PATH}"
)

# [external]
# websockets
add_library(duole_websockets STATIC IMPORTED GLOBAL)
set_target_properties(duole_websockets PROPERTIES
  IMPORTED_LOCATION "${EXTERNAL_PATH}/libwebsockets/prebuilt/${SYSTEM_OS}/libwebsockets.a"
)
# websockets-include
set(WEBSOCKETS_INCLUDE_PATH)
list(APPEND set(WEBSOCKETS_INCLUDE_PATH) "${EXTERNAL_PATH}/libwebsockets/include/${SYSTEM_OS}")
set_target_properties(duole_websockets PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${WEBSOCKETS_INCLUDE_PATH}"
)
