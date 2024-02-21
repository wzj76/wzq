/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2013-2016 Chukong Technologies Inc.
Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#include <stdlib.h>
#include <jni.h>
#include <android/log.h>
#include <string>
#include "platform/android/jni/JniHelper.h"
#include "platform/android/CCFileUtils-android.h"
#include "android/asset_manager_jni.h"
#include "platform/android/jni/Java_org_cocos2dx_lib_Cocos2dxHelper.h"

#include "base/ccUTF8.h"

#define  LOG_TAG    "Java_org_cocos2dx_lib_Cocos2dxHelper.cpp"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

static const std::string className = "org.cocos2dx.lib.Cocos2dxHelper";

static EditTextCallback s_editTextCallback = nullptr;
static void* s_ctx = nullptr;

static int __deviceSampleRate = 44100;
static int __deviceAudioBufferSizeInFrames = 192;

static std::string g_apkPath;

// EditArea: Added by Joi Yao.
static EditAreaCallback s_editAreaCallback = nullptr;
static void* s_pea = nullptr;
// ImagePicker: Added by Joi Yao.
static ImagePickerCallback s_imagePickerCallback = nullptr;
static void* s_pip = nullptr;

using namespace cocos2d;
using namespace std;

extern "C" {

    JNIEXPORT void JNICALL Java_org_cocos2dx_lib_Cocos2dxHelper_nativeSetContext(JNIEnv*  env, jobject thiz, jobject context, jobject assetManager) {
        JniHelper::setClassLoaderFrom(context);
        FileUtilsAndroid::setassetmanager(AAssetManager_fromJava(env, assetManager));
    }

    JNIEXPORT void JNICALL Java_org_cocos2dx_lib_Cocos2dxHelper_nativeSetAudioDeviceInfo(JNIEnv*  env, jobject thiz, jboolean isSupportLowLatency, jint deviceSampleRate, jint deviceAudioBufferSizeInFrames) {
        __deviceSampleRate = deviceSampleRate;
        __deviceAudioBufferSizeInFrames = deviceAudioBufferSizeInFrames;
        LOGD("nativeSetAudioDeviceInfo: sampleRate: %d, bufferSizeInFrames: %d", __deviceSampleRate, __deviceAudioBufferSizeInFrames);
    }

    JNIEXPORT void JNICALL Java_org_cocos2dx_lib_Cocos2dxHelper_nativeSetEditTextDialogResult(JNIEnv * env, jobject obj, jbyteArray text) {
        jsize  size = env->GetArrayLength(text);

        if (size > 0) {
            jbyte * data = (jbyte*)env->GetByteArrayElements(text, 0);
            char* buffer = (char*)malloc(size+1);
            if (buffer != nullptr) {
                memcpy(buffer, data, size);
                buffer[size] = '\0';
                // pass data to edittext's delegate
                if (s_editTextCallback) s_editTextCallback(buffer, s_ctx);
                free(buffer);
            }
            env->ReleaseByteArrayElements(text, data, 0);
        } else {
            if (s_editTextCallback) s_editTextCallback("", s_ctx);
        }
    }
    
	// EditArea: Added by Joi Yao.
    JNIEXPORT void JNICALL Java_org_cocos2dx_lib_Cocos2dxHelper_nativeSetEditAreaDialogResult(JNIEnv * env, jobject obj, jbyteArray text) {
        jsize  size = env->GetArrayLength(text);
        
        if (size > 0) {
            jbyte * data = (jbyte*)env->GetByteArrayElements(text, 0);
            char* buffer = (char*)malloc(size+1);
            if (buffer != nullptr) {
                memcpy(buffer, data, size);
                buffer[size] = '\0';
                // pass data to edittext's delegate
                if (s_editAreaCallback) s_editAreaCallback(buffer, s_pea);
                free(buffer);
            }
            env->ReleaseByteArrayElements(text, data, 0);
        } else {
            if (s_editAreaCallback) s_editAreaCallback("", s_pea);
        }
    }
    // ImagePicker: Added by Joi Yao.
    JNIEXPORT void JNICALL Java_org_cocos2dx_lib_Cocos2dxHelper_nativeSetImagePickerResult(JNIEnv * env, jobject obj, jbyteArray text, jboolean succeed) {
        jsize  size = env->GetArrayLength(text);
        
        if (size > 0) {
            jbyte * data = (jbyte*)env->GetByteArrayElements(text, 0);
            char* buffer = (char*)malloc(size+1);
            if (buffer != nullptr) {
                memcpy(buffer, data, size);
                buffer[size] = '\0';
                // pass data to delegate
                if (s_imagePickerCallback) s_imagePickerCallback(buffer, succeed, s_pip);
                free(buffer);
            }
            env->ReleaseByteArrayElements(text, data, 0);
        } else {
            if (s_imagePickerCallback) s_imagePickerCallback("", succeed, s_pip);
        }
    }
}

const char * getApkPath() {
    if (g_apkPath.empty())
    {
        g_apkPath = JniHelper::callStaticStringMethod(className, "getAssetsPath");
    }

    return g_apkPath.c_str();
}

std::string getPackageNameJNI() {
    return JniHelper::callStaticStringMethod(className, "getCocos2dxPackageName");
}

int getObbAssetFileDescriptorJNI(const char* path, long* startOffset, long* size) {
    JniMethodInfo methodInfo;
    int fd = 0;
    
    if (JniHelper::getStaticMethodInfo(methodInfo, className.c_str(), "getObbAssetFileDescriptor", "(Ljava/lang/String;)[J")) {
        jstring stringArg = methodInfo.env->NewStringUTF(path);
        jlongArray newArray = (jlongArray)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID, stringArg);
        jsize theArrayLen = methodInfo.env->GetArrayLength(newArray);
        
        if (theArrayLen == 3) {
            jboolean copy = JNI_FALSE;
            jlong *array = methodInfo.env->GetLongArrayElements(newArray, &copy);
            fd = static_cast<int>(array[0]);
            *startOffset = array[1];
            *size = array[2];
            methodInfo.env->ReleaseLongArrayElements(newArray, array, 0);
        }
        
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
        methodInfo.env->DeleteLocalRef(stringArg);
    }
    
    return fd;
}

int getDeviceSampleRate()
{
    return __deviceSampleRate;
}

int getDeviceAudioBufferSizeInFrames()
{
    return __deviceAudioBufferSizeInFrames;
}

void conversionEncodingJNI(const char* src, int byteSize, const char* fromCharset, char* dst, const char* newCharset)
{
    JniMethodInfo methodInfo;

    if (JniHelper::getStaticMethodInfo(methodInfo, className.c_str(), "conversionEncoding", "([BLjava/lang/String;Ljava/lang/String;)[B")) {
        jbyteArray strArray = methodInfo.env->NewByteArray(byteSize);
        methodInfo.env->SetByteArrayRegion(strArray, 0, byteSize, reinterpret_cast<const jbyte*>(src));

        jstring stringArg1 = methodInfo.env->NewStringUTF(fromCharset);
        jstring stringArg2 = methodInfo.env->NewStringUTF(newCharset);

        jbyteArray newArray = (jbyteArray)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID, strArray, stringArg1, stringArg2);
        jsize theArrayLen = methodInfo.env->GetArrayLength(newArray);
        methodInfo.env->GetByteArrayRegion(newArray, 0, theArrayLen, (jbyte*)dst);

        methodInfo.env->DeleteLocalRef(strArray);
        methodInfo.env->DeleteLocalRef(stringArg1);
        methodInfo.env->DeleteLocalRef(stringArg2);
        methodInfo.env->DeleteLocalRef(newArray);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }
}

// EditArea: Added by Joi Yao.
void showEditAreaDialogJNI(const char* title, const char* placeholder, const char* text, int maxLength, EditAreaCallback callback, void* pea)
{
    s_editAreaCallback = callback;
    s_pea = pea;
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, className.c_str(), "showEditAreaDialog", "(Ljava/lang/String;Ljava/lang/String;[BI)V")) {
        jstring stringArg1;
        
        if (!title) {
            stringArg1 = t.env->NewStringUTF("");
        } else {
            stringArg1 = t.env->NewStringUTF(title);
        }
        
        jstring stringArg2 = t.env->NewStringUTF(placeholder);
        int size = strlen(text);
        jbyteArray byteArg = t.env->NewByteArray(size);
        t.env->SetByteArrayRegion(byteArg, 0, size, (jbyte *)text);
        
        t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg1, stringArg2, byteArg, maxLength);
        
        t.env->DeleteLocalRef(stringArg1);
        t.env->DeleteLocalRef(stringArg2);
        t.env->DeleteLocalRef(byteArg);
        t.env->DeleteLocalRef(t.classID);
    }
}

// ImagePicker: Added by Joi Yao.
void showImagePickerJNI(int ratioX, int ratioY, int width, int height, bool allowsEditing, ImagePickerCallback callback, void* pip)
{
    s_imagePickerCallback = callback;
    s_pip = pip;
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, className.c_str(), "showImagePicker", "(IIIIZ)V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, ratioX, ratioY, width, height, allowsEditing);
        t.env->DeleteLocalRef(t.classID);
    }
}

void resetEditTextDialog(EditTextCallback callback, void* ctx) {
    if (s_editTextCallback == callback && s_ctx == ctx) {
    	s_editTextCallback = nullptr;
    	s_ctx = nullptr;
    }
}
// EditArea: Added by Joi Yao.
void resetEditAreaDialog(EditAreaCallback callback, void* pea) {
    if (s_editAreaCallback == callback && s_pea == pea) {
        s_editAreaCallback = nullptr;
        s_pea = nullptr;
    }
}
// ImagePicker: Added by Joi Yao
void resetImagePicker(ImagePickerCallback callback, void* pip) {
    if (s_imagePickerCallback == callback && s_pip == pip)
    {
        s_imagePickerCallback = nullptr;
        s_pip = nullptr;
    }
}