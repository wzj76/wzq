LOCAL_PATH := $(call my-dir)
# 项目根目录
ROOT_PATH := $(LOCAL_PATH)/../../../../..

# 添加三方库路径
$(call import-add-path,$(ROOT_PATH)/../../duole_client_base/cocos2d-duole)
$(call import-add-path,$(ROOT_PATH)/../../duole_client_base/cocos2d-duole/external)
$(call import-add-path,$(ROOT_PATH)/../../duole_client_base/cocos2d-duole/cocos)
$(call import-add-path,$(ROOT_PATH)/../../duole_client_base/gamebase)
$(call import-add-path,$(ROOT_PATH)/../../duole_client_base/third-sdks)

include $(CLEAR_VARS)
LOCAL_MODULE := phzgame_shared
LOCAL_MODULE_FILENAME := libphzgame

# 源代码设置；
# Class目录；
define classes-cpp-files
$(patsubst jni/%,%,$(shell find $(ROOT_PATH)/Classes -name "*.cpp")) \
$(patsubst jni/%,%,$(shell find $(ROOT_PATH)/Classes -name "*.cc")) \
$(patsubst jni/%,%,$(shell find $(ROOT_PATH)/Classes -name "*.c"))
endef
# gamebase/eff_comm
define libs-effcommon-cpp-files
$(patsubst jni/%,%,$(shell find $(ROOT_PATH)/../../duole_client_base/gamebase/eff_comm -name "*.cpp"))
endef
# gamebase/rapidxml
define libs-rapidxml-cpp-files
$(patsubst jni/%,%,$(shell find $(ROOT_PATH)/../../duole_client_base/gamebase/rapidxml -name "*.cpp"))
endef
# gamebase/socketx
define libs-socketx-cpp-files
$(patsubst jni/%,%,$(shell find $(ROOT_PATH)/../../duole_client_base/gamebase/socketx -name "*.cpp"))
endef

LOCAL_SRC_FILES := $(LOCAL_PATH)/cocos2dcpp/main.cpp \
                   $(LOCAL_PATH)/cocos2dcpp/PlatformFunction.cpp \
                   $(call classes-cpp-files) \
                   $(call libs-effcommon-cpp-files) \
                   $(call libs-rapidxml-cpp-files) \
                   $(call libs-socketx-cpp-files) 

LOCAL_C_INCLUDES := $(ROOT_PATH)/Classes \
                    $(ROOT_PATH)/../../duole_client_base/gamebase \
                    $(ROOT_PATH)/../../duole_client_base/gamebase/google \
                    $(ROOT_PATH)/../../duole_client_base/third-sdks \

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END

LOCAL_STATIC_LIBRARIES := cc_static
LOCAL_STATIC_LIBRARIES += cclua_static
LOCAL_STATIC_LIBRARIES += cclua_android

LOCAL_STATIC_LIBRARIES += protobuf_static
LOCAL_STATIC_LIBRARIES += bugly_crashreport_cocos_static
# bugly for lua
LOCAL_STATIC_LIBRARIES += bugly_agent_cocos_static_lua
# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module, cocos)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
$(call import-module, scripting/lua-bindings/proj.android)
$(call import-module, google)
$(call import-module, bugly/plugin)
# bugly for lua
$(call import-module, bugly/plugin/lua)
