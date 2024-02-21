# gamebase
list(APPEND duole_clientbase_libs "duole_gamebase")
# sdks
list(APPEND duole_clientbase_libs "duole_sdks_platform")
# external
list(APPEND duole_clientbase_libs "duole_protobuf")
list(APPEND duole_clientbase_libs "duole_iconv")
# third-sdks
# list(APPEND duole_clientbase_libs "duole_bugly")
# list(APPEND duole_clientbase_libs "${THIRDSDKS_ROOT_PATH}/bugly/buglysdk/android/prebuilt/${ANDROID_ABI}/libBugly.so")
list(APPEND duole_clientbase_libs "duole_bugly_v2")
