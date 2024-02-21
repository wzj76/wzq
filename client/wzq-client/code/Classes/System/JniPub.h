#pragma once

#include "cocos2d.h"
#include <string>

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define OS_ANDROID			//定义是否在android上运行
#endif//

#ifdef OS_ANDROID
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#endif//OS_ANDROID

#ifdef OS_ANDROID

void JNI_GetStringField(JNIEnv* env, jclass clazz, jobject obj, const char* name, std::string& strVal);
int  JNI_GetIntField(JNIEnv* env, jclass clazz, jobject obj, const char* name);
long JNI_GetLongField(JNIEnv* env, jclass clazz, jobject obj, const char* name);
void JNI_SetStringField(JNIEnv* env, jclass clazz, jobject obj, const char* name, const char* szVal);
void JNI_SetIntField(JNIEnv* env, jclass clazz, jobject obj, const char* name, jint jnVal);
void JNI_SetLongField(JNIEnv* env, jclass clazz, jobject obj, const char* name, jlong jlVal);
void JNI_SetFloatField(JNIEnv* env, jclass clazz, jobject obj, const char* name, jfloat jlVal);

#endif//OS_ANDROID

bool JNI_CallVoidMethodReturnString(const char* pszClassName, const char* pszMethod, std::string& strReturn);
bool JNI_CallVoidMethodReturnVoid(const char* pszClassName, const char* pszMethod);
bool JNI_CallVoidMethodReturnBoolean(const char* pszClassName, const char* pszMethod, bool& bReturn);
bool JNI_CallVoidMethodReturnInt(const char* pszClassName, const char* pszMethod, int& nReturn);
bool JNI_CallStringMethodReturnString(const char* pszClassName, const char* pszMethod, const char* pszParam, std::string& strReturn);
