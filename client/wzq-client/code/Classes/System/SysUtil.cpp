#include "SysUtil.h"
#include "JniPub.h"
#include "cocos2d.h"

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define OS_ANDROID			//定义是否在android上运行
#endif//

#ifdef OS_ANDROID
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#endif//OS_ANDROID

#define CLASS_NAME "com/duole/jniutil/SysUtil"

USING_NS_CC;
using namespace std;

bool CSysUtil::GetMobileID(string& strMobileID)
{
	log("GetMobileID...");
#ifdef OS_ANDROID
	JniMethodInfo t;
	if(JniHelper::getStaticMethodInfo(t, CLASS_NAME, "rtnObject", "()Ljava/lang/Object;"))
	{
		jobject jobj;
		jobj = t.env->CallStaticObjectMethod(t.classID, t.methodID);

		if(JniHelper::getStaticMethodInfo(t, CLASS_NAME, "getMobileID", "()Ljava/lang/String;"))
		{
			jstring jInfo = static_cast<jstring>(t.env->CallStaticObjectMethod(t.classID, t.methodID));
			if(jInfo)
			{
				const char* pszChars = t.env->GetStringUTFChars(jInfo, nullptr);
				if(strlen(pszChars) > 2)
				{
					strMobileID.assign(pszChars);
					log("GetMobileID:%s", pszChars);
					t.env->ReleaseStringUTFChars(jInfo, pszChars);
					return true;
				}				
				t.env->ReleaseStringUTFChars(jInfo, pszChars);
			}
		}
	}
#endif//OS_ANDROID
	return false;
}

//取得电话号码
bool CSysUtil::GetPhoneNumber(string& strPhoneNumber)
{
	log("GetPhoneNumber...");
#ifdef OS_ANDROID
	JniMethodInfo t;
	if(JniHelper::getStaticMethodInfo(t, CLASS_NAME, "rtnObject", "()Ljava/lang/Object;"))
	{
		jobject jobj;
		jobj = t.env->CallStaticObjectMethod(t.classID, t.methodID);

		if(JniHelper::getMethodInfo(t, CLASS_NAME, "getPhoneNumber", "()Ljava/lang/String;"))
		{
			jstring jInfo = static_cast<jstring>(t.env->CallObjectMethod(jobj, t.methodID));
			if(jInfo)
			{
				const char* pszChars = t.env->GetStringUTFChars(jInfo, nullptr);
				if(strlen(pszChars) > 2)
				{
					strPhoneNumber.assign(pszChars);
					log("GetPhoneNumber:%s", pszChars);
					t.env->ReleaseStringUTFChars(jInfo, pszChars);
					return true;
				}				
				t.env->ReleaseStringUTFChars(jInfo, pszChars);
			}
		}
	}
#endif//OS_ANDROID
	return false;
}

//取得IMSI，可通过IMSI区分运营商
//46000/46002:中国移动 46001:中国联通 46003:中国电信
bool CSysUtil::GetIMSI(string& strIMSI)
{
	log("GetIMSI...");
#ifdef OS_ANDROID
	JniMethodInfo t;
	if(JniHelper::getStaticMethodInfo(t, CLASS_NAME, "rtnObject", "()Ljava/lang/Object;"))
	{
		jobject jobj;
		jobj = t.env->CallStaticObjectMethod(t.classID, t.methodID);

		if(JniHelper::getMethodInfo(t, CLASS_NAME, "getIMSI", "()Ljava/lang/String;"))
		{
			jstring jInfo = static_cast<jstring>(t.env->CallObjectMethod(jobj, t.methodID));
			if(jInfo)
			{
				const char* pszChars = t.env->GetStringUTFChars(jInfo, nullptr);
				if(strlen(pszChars) > 2)
				{
					strIMSI.assign(pszChars);
					log("GetIMSI:%s", pszChars);
					t.env->ReleaseStringUTFChars(jInfo, pszChars);
					return true;
				}				
				t.env->ReleaseStringUTFChars(jInfo, pszChars);
			}
		}
	}
#endif//OS_ANDROID
	return false;
}

bool CSysUtil::GetCacheDir(string& strCacheDir)
{
	return JNI_CallVoidMethodReturnString(CLASS_NAME, "getCacheDir", strCacheDir);
}

bool CSysUtil::GetExternalCacheDir(string& strCacheDir)
{
	return JNI_CallVoidMethodReturnString(CLASS_NAME, "getExternalCacheDir", strCacheDir);
}

bool CSysUtil::GetFilesDir(string& strCacheDir)
{
	return JNI_CallVoidMethodReturnString(CLASS_NAME, "getFilesDir", strCacheDir);
}

bool CSysUtil::GetExternalStorageDirectory(string& strDir)
{
	return JNI_CallVoidMethodReturnString(CLASS_NAME, "getExternalStorageDirectory", strDir);
}

bool CSysUtil::GetPackageName(string& strName)
{
	return JNI_CallVoidMethodReturnString(CLASS_NAME, "getPackageName", strName);
}

bool CSysUtil::GetVersionName(string& strVersion)
{
	return JNI_CallVoidMethodReturnString(CLASS_NAME, "getVersionName", strVersion);
}

bool CSysUtil::GetApplicationMetaData(const char* pszMetaName, string& strMetaVal)
{
	return JNI_CallStringMethodReturnString(CLASS_NAME, "getApplicationMetaData", pszMetaName, strMetaVal);
}

bool CSysUtil::GetBuildModel(string& strModel)
{
	return JNI_CallVoidMethodReturnString(CLASS_NAME, "getBuildModel", strModel);
}

bool CSysUtil::GetBuildManufacturer(string& strManufact)
{
	return JNI_CallVoidMethodReturnString(CLASS_NAME, "getBuildManufacturer", strManufact);
}

bool CSysUtil::GoToURL(const char* pszURL, string& strVale)
{
	return JNI_CallStringMethodReturnString(CLASS_NAME, "ShowUrl", pszURL, strVale);
}

bool CSysUtil::GetWriteablePath(string& strPath)
{
	string strTmp = FileUtils::getInstance()->getWritablePath();
	strPath.assign(strTmp);
	return true;
}

bool CSysUtil::AppendDataToFile(string strFileName, const void* pBuf, int nSize)
{
	bool bRet = false;

	string strPath = FileUtils::getInstance()->getWritablePath();
	string strFile = strPath + strFileName;

	FILE* fp = nullptr;
	do
	{
		fp = fopen(strFile.c_str(), "a+");
		if(!fp)
			break;
		fseek(fp, 0, SEEK_END);
		size_t nBytes = fwrite(pBuf, 1, nSize, fp);
		if(nBytes != nSize)
			break;
		bRet = true;
	}while(0);
	if(fp)
		fclose(fp);

	return bRet;
}

//判断网络是否有效
bool CSysUtil::IsNetworkConnected()
{
	bool bRet = false;
#ifdef OS_ANDROID
	JNI_CallVoidMethodReturnBoolean(CLASS_NAME, "isNetworkConnected", bRet); 
#endif
	return bRet;
}

//判断WIFI网络是否可用
bool CSysUtil::IsWifiConnected()
{
	bool bRet = false;
#ifdef testOS_ANDROID
	JNI_CallVoidMethodReturnBoolean(CLASS_NAME, "isWifiConnected", bRet); 
#endif
	return bRet;
}

//判断MOBILE网络是否可用
bool CSysUtil::IsMobileConnected()
{
	bool bRet = false;
#ifdef OS_ANDROID
	JNI_CallVoidMethodReturnBoolean(CLASS_NAME, "isMobileConnected", bRet); 
#endif
	return bRet;
}

//获取当前网络连接的类型信息
int CSysUtil::GetConnectedType()
{
	int nRet = -1;
#ifdef testOS_ANDROID
	JNI_CallVoidMethodReturnInt(CLASS_NAME, "getConnectedType", nRet); 
#endif
	return nRet;
}

//获取当前网络连接的强度
int CSysUtil::GetSignalStrength()
{
    int nRet = -1;
#ifdef testOS_ANDROID
    if (GetConnectedType() == NETWORK_TYPE::NETWORK_TYPE_MOBILE)
    {
        JNI_CallVoidMethodReturnInt(CLASS_NAME, "getSignalStrengthGsm", nRet);
    }
    else if (GetConnectedType() == NETWORK_TYPE::NETWORK_TYPE_WIFI)
    {
        JNI_CallVoidMethodReturnInt(CLASS_NAME, "getSignalStrengthWifi", nRet);
    }
#endif
    return nRet;
}

#ifdef OS_ANDROID

#include <signal.h>

static struct sigaction old_sa[NSIG];

void android_sigaction(int signal, siginfo_t *info, void *reserved)
{
	log("android_sigaction:%d", signal);
	if(signal == SIGPIPE)
	{
        //要忽略此信号
		return;
	}
 
	JniMethodInfo t;
	if(JniHelper::getStaticMethodInfo(t, CLASS_NAME, "nativeCrashed", "()V"))
	{
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
	}

	//恢复原有信号处理
	old_sa[signal].sa_handler(signal);
}

#endif

bool CSysUtil::s_bCrashSetupCompleted = false;

//crash handler init
void CSysUtil::CrashHandlerSetup()
{
	if(s_bCrashSetupCompleted) return;
#ifdef OS_ANDROID
	struct sigaction handler;
	memset(&handler, 0, sizeof(handler));
	handler.sa_sigaction = android_sigaction;
	handler.sa_flags = SA_RESETHAND;
#define CATCHSIG(X) sigaction(X, &handler, &old_sa[X])
	CATCHSIG(SIGILL);
	CATCHSIG(SIGABRT);
	CATCHSIG(SIGBUS);
	CATCHSIG(SIGFPE);
	CATCHSIG(SIGSEGV);
	CATCHSIG(SIGSTKFLT);
	CATCHSIG(SIGPIPE);
#endif
	s_bCrashSetupCompleted = true;
}
