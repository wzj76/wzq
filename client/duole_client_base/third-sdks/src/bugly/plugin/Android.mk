LOCAL_PATH := $(call my-dir)

# --- libBugly.so ---
include $(CLEAR_VARS)
LOCAL_MODULE := bugly_native_prebuilt
LOCAL_SRC_FILES := $(LOCAL_PATH)/../buglysdk/android/prebuilt/$(TARGET_ARCH_ABI)/libBugly.so
include $(PREBUILT_SHARED_LIBRARY)
# --- end ---

include $(CLEAR_VARS)
LOCAL_MODULE := bugly_crashreport_cocos_static

LOCAL_MODULE_FILENAME := libcrashreport

LOCAL_CPP_EXTENSION := .mm .cpp .cc
LOCAL_CFLAGS += -x c++

LOCAL_SRC_FILES := CrashReport.mm 

LOCAL_C_INCLUDES := $(LOCAL_PATH)\
$(LOCAL_PATH)/../ \
$(LOCAL_PATH)/../../ \
$(LOCAL_PATH)/../../../cocos2d-duole/cocos/base \
$(LOCAL_PATH)/../../../cocos2d-duole/cocos \
$(LOCAL_PATH)/../../../cocos2d-duole/cocos/2d \
$(LOCAL_PATH)/../../../cocos2d-duole/cocos/2d/platform/android \
$(LOCAL_PATH)/../../../cocos2d-duole/cocos/platform/android \
$(LOCAL_PATH)/../../../cocos2d-duole/cocos/math/kazmath \
$(LOCAL_PATH)/../../../cocos2d-duole/cocos/physics \
$(LOCAL_PATH)/../../cocos2dx \
$(LOCAL_PATH)/../../cocos2dx/include \
$(LOCAL_PATH)/../../cocos2dx/platform/android \
$(LOCAL_PATH)/../../cocos2dx/kazmath/include \
$(LOCAL_PATH)/../../../cocos2d-duole/external

include $(BUILD_STATIC_LIBRARY)