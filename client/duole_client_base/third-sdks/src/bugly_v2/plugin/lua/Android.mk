LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := bugly_agent_cocos_static_lua

LOCAL_MODULE_FILENAME := libbuglyagentlua

LOCAL_SRC_FILES := BuglyLuaAgent.cpp

# Maybe you need modify the head files according to the version of cocos engine be used
# This head files Cocos/frameworks/cocos2d-x-3.6
#
LOCAL_C_INCLUDES := $(LOCAL_PATH)\
$(LOCAL_PATH)/../ \
$(LOCAL_PATH)/../../ \
$(LOCAL_PATH)/../../../ \
$(LOCAL_PATH)/../../../../cocos2d-duole/cocos \
$(LOCAL_PATH)/../../../../cocos2d-duole/cocos/base \
$(LOCAL_PATH)/../../../../cocos2d-duole/cocos/platform/android \
$(LOCAL_PATH)/../../../../cocos2d-duole/cocos/scripting/lua-bindings/manual \
$(LOCAL_PATH)/../../../../cocos2d-duole/cocos/math/kazmath \
$(LOCAL_PATH)/../../../../cocos2d-duole/cocos/physics \
$(LOCAL_PATH)/../../../../cocos2d-duole/external \
$(LOCAL_PATH)/../../../../cocos2d-duole/external/lua/luajit/include\
$(LOCAL_PATH)/../../../../cocos2d-duole/external/lua/tolua\
$(LOCAL_PATH)/../../../cocos2dx \
$(LOCAL_PATH)/../../../cocos2dx/include \
$(LOCAL_PATH)/../../../cocos2dx/platform/android \
$(LOCAL_PATH)/../../../cocos2dx/kazmath/include \
$(LOCAL_PATH)/../../../cocos/2d \
$(LOCAL_PATH)/../../../cocos/2d/platform/android \
$(LOCAL_PATH)/../../../../cocos2d-duole/scripting/lua/cocos2dx_support \
$(LOCAL_PATH)/../../../../cocos2d-duole/scripting/lua/lua \
$(LOCAL_PATH)/../../../../cocos2d-duole/scripting/lua/tolua \

                                  
include $(BUILD_STATIC_LIBRARY)