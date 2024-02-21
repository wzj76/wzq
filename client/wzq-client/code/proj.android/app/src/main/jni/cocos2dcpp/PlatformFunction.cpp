//
//  PlatformFunction.cpp
//  GouJi
//
//  Created by mtt on 14-9-29.
//
//

#include "PlatformFunction.h"
#include "Core/AppVersion.h"
#include "Core/AppEvent.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include "AppDelegate.h"
#include "jni/JniHelper.h"
#include "Core/AppRes.h"
#include "Util/PHZReaderWriter.h"
#include "Core/AppDeveloper.h"

USING_NS_CC;
using namespace std;

#define APP_ACTIVITY "com/duole/paohuzihd/AppActivity"
#define CLASS_NAME "com/duole/jniutil/PlatformFunction"
#define CLASS_DLLAUNCHER "com/duole/sdk/launcher/DLLauncherUtil"
#define SYS_UTIL "com/duole/jniutil/SysUtil"
#define UPDATE_CLASS_NAME "com/duole/jniutil/CheckUpdate"
#define PERMISSIONS_TIIL "com/duole/paohuzihd/PermissionsUtil"

static string s_sEventOnFinish = "";
static string s_sEventAuthFinish = "";
static string m_sSchemePath = "";
static string m_sSchemeData = "";
void PlatformFunction::openURL(const string &sUrl)
{
    JniMethodInfo obMethodInfo;
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       CLASS_NAME,
                                       "openURL",
                                       "(Ljava/lang/String;)V"))
    {
        jstring url = obMethodInfo.env->NewStringUTF(sUrl.c_str());

        obMethodInfo.env->CallStaticVoidMethod(obMethodInfo.classID,
                                               obMethodInfo.methodID,
                                               url);

        obMethodInfo.env->DeleteLocalRef(url);
        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
}

// 用户勾选或取消勾选隐私政策； ,0 -- 未勾选 1-- 勾选
void PlatformFunction::setPrivacyFlag(const int &flag)
{
    JniMethodInfo obMethodInfo;
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       CLASS_DLLAUNCHER,
                                       "setPrivacyCheckBox",
                                       "(I)V"))
    {

        jint nFlag = (jint)flag;

        obMethodInfo.env->CallStaticVoidMethod(obMethodInfo.classID,
                                                       obMethodInfo.methodID,
                                                       nFlag
                                                       );

         obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
}

// 展示《多乐游戏⽤户服务协议》
void PlatformFunction::showPrivacyAgreement()
{
    JniMethodInfo obMethodInfo;
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
    								   CLASS_DLLAUNCHER,
                                       "showPrivacyAgreement",
                                       "()V"))
    {
        obMethodInfo.env->CallStaticVoidMethod(obMethodInfo.classID,
                                               obMethodInfo.methodID);

        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
}

// 展示《多乐游戏隐私保护指引》
void PlatformFunction::showPrivacyGuide()
{
    JniMethodInfo obMethodInfo;
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
    								   CLASS_DLLAUNCHER,
                                       "showPrivacyGuide",
                                       "()V"))
    {
        obMethodInfo.env->CallStaticVoidMethod(obMethodInfo.classID,
                                               obMethodInfo.methodID);

        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
}

// 展示《多乐游戏未成年隐私保护指引》
void PlatformFunction::showPrivacyGuideChildren()
{
    JniMethodInfo obMethodInfo;
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
    								   CLASS_DLLAUNCHER,
                                       "showPrivacyGuideChildren",
                                       "()V"))
    {
        obMethodInfo.env->CallStaticVoidMethod(obMethodInfo.classID,
                                               obMethodInfo.methodID);

        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
}

void PlatformFunction::openApplication(const string &sLinkUrl, const string &sDownloadUrl)
{
    JniMethodInfo obMethodInfo;
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       CLASS_NAME,
                                       "openApplication",
                                       "(Ljava/lang/String;Ljava/lang/String;)V"))
    {
        jstring linkUrl = obMethodInfo.env->NewStringUTF(sLinkUrl.c_str());
        jstring downloadUrl = obMethodInfo.env->NewStringUTF(sDownloadUrl.c_str());
        obMethodInfo.env->CallStaticVoidMethod(obMethodInfo.classID,
                                               obMethodInfo.methodID,
                                               linkUrl,
                                               downloadUrl);
        obMethodInfo.env->DeleteLocalRef(linkUrl);
        obMethodInfo.env->DeleteLocalRef(downloadUrl);
        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
}

bool PlatformFunction::canOpenApplication(const std::string &sLinkUrl)
{
	JniMethodInfo obMethodInfo;
	jboolean bRet = false;
	if (JniHelper::getStaticMethodInfo(obMethodInfo,
	                                   CLASS_NAME,
	                                   "canOpenApplication",
	                                   "(Ljava/lang/String;)Z"))
	{
		jstring url = obMethodInfo.env->NewStringUTF(sLinkUrl.c_str());

		bRet = obMethodInfo.env->CallStaticBooleanMethod(obMethodInfo.classID,
		                                               	 obMethodInfo.methodID,
		                                                 url);

		obMethodInfo.env->DeleteLocalRef(url);
		obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
	}
	return bRet == JNI_TRUE ? true : false;
}

void PlatformFunction::installFile()
{
}

float PlatformFunction::getBatteryLevel()
{
    JniMethodInfo obMethodInfo;
    jfloat fRet = 0;
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       CLASS_NAME,
                                       "getBatteryLevel",
                                       "()F"))
    {
        fRet = obMethodInfo.env->CallStaticFloatMethod(obMethodInfo.classID,
                                                       obMethodInfo.methodID);

        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
    return fRet;
}

void PlatformFunction::share(SocialPlatform ePlatform,
							 const std::string &sShareURL,
                             const string &sText,
                             const string &sTitle,
                             const string &sEventOnFinish,
                             const int taskId)
{
    appModel->m_pShareModel->setShareTimeStamp();
    s_sEventOnFinish = sEventOnFinish;

    JniMethodInfo obMethodInfo;
    
    stringstream stream;
    stream<<taskId;
    string stemp=stream.str();
    
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       CLASS_NAME,
                                       "share",
                                       "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V"))
    {
    	jint platform = (jint)ePlatform;
    	jstring share = obMethodInfo.env->NewStringUTF(sShareURL.c_str());
    	jstring text = obMethodInfo.env->NewStringUTF(sText.c_str());
    	jstring title = obMethodInfo.env->NewStringUTF(sTitle.c_str());
        jstring temp = obMethodInfo.env->NewStringUTF(stemp.c_str());

        obMethodInfo.env->CallStaticVoidMethod(obMethodInfo.classID,
                                               obMethodInfo.methodID,
                                               platform,
                                               share,
                                               text,
                                               title,
                                               temp);

        obMethodInfo.env->DeleteLocalRef(share);
        obMethodInfo.env->DeleteLocalRef(text);
        obMethodInfo.env->DeleteLocalRef(title);
        obMethodInfo.env->DeleteLocalRef(temp);
        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
}

void PlatformFunction::shareWechatMiniGame(SocialPlatform ePlatform,
                      const std::string &sShareURL,
                      const std::string &sText,
                      const std::string &sTitle,
                      const std::string &sEventOnFinish,
                      const int taskId)
{
    appModel->m_pShareModel->setShareTimeStamp();
    s_sEventOnFinish = sEventOnFinish;

    JniMethodInfo obMethodInfo;
    
    stringstream stream;
    stream<<taskId;
    string stemp=stream.str();
    
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       CLASS_NAME,
                                       "shareMiniGame",
                                       "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V"))
    {
      jint platform = (jint)ePlatform;
      jstring share = obMethodInfo.env->NewStringUTF(sShareURL.c_str());
      jstring text = obMethodInfo.env->NewStringUTF(sText.c_str());
      jstring title = obMethodInfo.env->NewStringUTF(sTitle.c_str());
        jstring temp = obMethodInfo.env->NewStringUTF(stemp.c_str());

        obMethodInfo.env->CallStaticVoidMethod(obMethodInfo.classID,
                                               obMethodInfo.methodID,
                                               platform,
                                               share,
                                               text,
                                               title,
                                               temp);

        obMethodInfo.env->DeleteLocalRef(share);
        obMethodInfo.env->DeleteLocalRef(text);
        obMethodInfo.env->DeleteLocalRef(title);
        obMethodInfo.env->DeleteLocalRef(temp);
        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
}

void PlatformFunction::shareImage(SocialPlatform ePlatform,
							      const std::string &sShareURL,
                                  const string &sAbsoluteImagePath,
                                  const int taskId,
                                  const string &sEventOnFinish
                                  )
{
    s_sEventOnFinish = sEventOnFinish;
    
    stringstream stream;
    stream<<taskId;
    string stemp=stream.str();
    
    JniMethodInfo obMethodInfo;
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       CLASS_NAME,
                                       "shareImage",
                                       "(ILjava/lang/String;Ljava/lang/String;)V"))
    {
    	jint platform = (jint)ePlatform;
    	jstring share = obMethodInfo.env->NewStringUTF(sShareURL.c_str());
    	jstring absoluteImagePath = obMethodInfo.env->NewStringUTF(sAbsoluteImagePath.c_str());
        jstring temp = obMethodInfo.env->NewStringUTF(stemp.c_str());

        obMethodInfo.env->CallStaticVoidMethod(obMethodInfo.classID,
                                               obMethodInfo.methodID,
                                               platform,
                                               share,
                                               absoluteImagePath,
                                               temp);

        obMethodInfo.env->DeleteLocalRef(share);
        obMethodInfo.env->DeleteLocalRef(absoluteImagePath);
        obMethodInfo.env->DeleteLocalRef(temp);
        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
}

void PlatformFunction::copyText(const std::string &sText)
{
	JniMethodInfo obMethodInfo;
	if (JniHelper::getStaticMethodInfo(obMethodInfo,
	                                   CLASS_NAME,
	                                   "copyText",
	                                   "(Ljava/lang/String;)V"))
	{
	    jstring url = obMethodInfo.env->NewStringUTF(sText.c_str());

	    obMethodInfo.env->CallStaticVoidMethod(obMethodInfo.classID,
	                                           obMethodInfo.methodID,
	                                           url);

	    obMethodInfo.env->DeleteLocalRef(url);
	    obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
	}
}

bool PlatformFunction::joinQQGroup(const std::string &sUrl)
{
	JniMethodInfo obMethodInfo;
	jboolean bRet = false;
	if (JniHelper::getStaticMethodInfo(obMethodInfo,
									   CLASS_NAME,
									   "joinQQGroup",
									   "(Ljava/lang/String;)Z"))
	{
		jstring url = obMethodInfo.env->NewStringUTF(sUrl.c_str());

		bRet = obMethodInfo.env->CallStaticBooleanMethod(obMethodInfo.classID,
														 obMethodInfo.methodID,
														 url);

		obMethodInfo.env->DeleteLocalRef(url);
		obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
	}
	return bRet == JNI_TRUE ? true : false;
}

int PlatformFunction::getNetType()
{
  JniMethodInfo obMethodInfo;
  jint bRet = 0;
  if (JniHelper::getStaticMethodInfo(obMethodInfo,
                     SYS_UTIL,
                     "getConnectedType",
                     "()I"))
  {

    bRet = obMethodInfo.env->CallStaticIntMethod(obMethodInfo.classID,
                         obMethodInfo.methodID);

    obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
  }
  return bRet;
}

int PlatformFunction::getSignalStrength()
{
  JniMethodInfo obMethodInfo;
  jint bRet = 0;
  if (JniHelper::getStaticMethodInfo(obMethodInfo,
                     SYS_UTIL,
                     "getSignalStrength",
                     "()I"))
  {

    bRet = obMethodInfo.env->CallStaticIntMethod(obMethodInfo.classID,
                         obMethodInfo.methodID);

    obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
  }
  return bRet;
}

// 打开设置界面
void PlatformFunction::openSetting()
{
    JniMethodInfo obMethodInfo;
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
    									SYS_UTIL,
                                       "doOpenSetting",
                                       "()V"))
    {
        obMethodInfo.env->CallStaticVoidMethod(obMethodInfo.classID,
                                               obMethodInfo.methodID);

        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
}

// 检查更新
void PlatformFunction::doManualCheckUpdate()
{
    JniMethodInfo obMethodInfo;
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       UPDATE_CLASS_NAME,
                                       "doManualCheckUpdate",
                                       "()V"))
    {
        obMethodInfo.env->CallStaticVoidMethod(obMethodInfo.classID,
                                               obMethodInfo.methodID);

        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
}

// 手动更新
void PlatformFunction::doManualUpdate()
{
    JniMethodInfo obMethodInfo;
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       UPDATE_CLASS_NAME,
                                       "doManualUpdate",
                                       "()V"))
    {
        obMethodInfo.env->CallStaticVoidMethod(obMethodInfo.classID,
                                               obMethodInfo.methodID);

        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
}

//反馈
void PlatformFunction::showFeedBack(const std::string &userID, const std::string &channel, const std::string &version, const std::string &device)
{
	JniMethodInfo obMethodInfo;
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       CLASS_NAME,
                                       "showFeedBack",
                                       "(Ljava/lang/String;Ljava/lang/String;)V"))
    {
    	jstring userid1 = obMethodInfo.env->NewStringUTF(userID.c_str());
    	jstring device1 = obMethodInfo.env->NewStringUTF(device.c_str());

        obMethodInfo.env->CallStaticVoidMethod(obMethodInfo.classID,
                                               obMethodInfo.methodID,
                                               userid1,
                                               device1);

        obMethodInfo.env->DeleteLocalRef(userid1);
        obMethodInfo.env->DeleteLocalRef(device1);
        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
}

// 评论
void PlatformFunction::showComment()
{
    JniMethodInfo obMethodInfo;
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       SYS_UTIL,
                                       "doOpenComment",
                                       "()V"))
    {
        obMethodInfo.env->CallStaticVoidMethod(obMethodInfo.classID,
                                               obMethodInfo.methodID);
        
        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
}

// 打开微信
bool PlatformFunction::openWeChat()
{
    jboolean bRet = false;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo obMethodInfo;
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       SYS_UTIL,
                                       "openWeChat",
                                       "()Z"))
    {
        bRet = obMethodInfo.env->CallStaticBooleanMethod(obMethodInfo.classID,
                                                         obMethodInfo.methodID);
        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
#endif
    return bRet == JNI_TRUE ? true : false;
}

void PlatformFunction::startPay(std::map<std::string, std::string> orderInfo, KKPAYTAYPE payType)
{
  JniMethodInfo obMethodInfo;

    switch(payType)
    {
        case PURCHASE_METHOD_TREASUREPAY:
        {
            string str_url;

            map<string, string>::iterator iter;
            iter = orderInfo.find("url");
            if(iter != orderInfo.end())
              str_url = orderInfo["url"];

            Value oData = Value(str_url.c_str());
            appModel->m_pRemoteModel->setWebChangeScreen(false);
            Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(NTF_SHOW_WEBVIEW, &oData);
            Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(EVT_COMMON_CHECK_DELIVER);
            
            // if (JniHelper::getStaticMethodInfo(obMethodInfo,
            //                            "com/duole/sdk/wechat/WeChatSdkUtil",
            //                            "doAliPay",
            //                            "(Ljava/lang/String;)V"))
            // {
            //     jstring str_url1 = obMethodInfo.env->NewStringUTF(str_url.c_str());

            //     obMethodInfo.env->CallStaticVoidMethod(obMethodInfo.classID,
            //                                            obMethodInfo.methodID,
            //                                            str_url1);

            //     obMethodInfo.env->DeleteLocalRef(str_url1);
            //     obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
            // }
            // Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(EVT_COMMON_CHECK_DELIVER);

        }break;
        case PURCHASE_METHOD_WWCXPAY:
        {            
            string partnerId;
            string prepayId;
            string nonceStr;
            string timeStamp;
            string package;
            string sign;
            // 调起微信支付
            map<string, string>::iterator iter;
            iter = orderInfo.find("partnerid");
            if(iter != orderInfo.end())
                partnerId = orderInfo["partnerid"];
            iter = orderInfo.find("prepayid");
            if(iter != orderInfo.end())
                prepayId = orderInfo["prepayid"];
            iter = orderInfo.find("noncestr");
            if(iter != orderInfo.end())
                nonceStr = orderInfo["noncestr"];
            iter = orderInfo.find("timestamp");
            if(iter != orderInfo.end())
                timeStamp = orderInfo["timestamp"];
            iter = orderInfo.find("package");
            if(iter != orderInfo.end())
                package = orderInfo["package"];
            iter = orderInfo.find("sign");
            if(iter != orderInfo.end())
                sign = orderInfo["sign"];

            if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       "com/duole/sdk/wechat/WeChatSdkUtil",
                                       "doSdkPay",
                                       "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V"))
            {
                jstring partnerId1 = obMethodInfo.env->NewStringUTF(partnerId.c_str());
                jstring prepayId1 = obMethodInfo.env->NewStringUTF(prepayId.c_str());
                jstring nonceStr1 = obMethodInfo.env->NewStringUTF(nonceStr.c_str());
                jstring timeStamp1 = obMethodInfo.env->NewStringUTF(timeStamp.c_str());
                jstring package1 = obMethodInfo.env->NewStringUTF(package.c_str());
                jstring sign1 = obMethodInfo.env->NewStringUTF(sign.c_str());

                obMethodInfo.env->CallStaticVoidMethod(obMethodInfo.classID,
                                                       obMethodInfo.methodID,
                                                       partnerId1,
                                                       prepayId1,
                                                       nonceStr1,
                                                       timeStamp1,
                                                       package1,
                                                       sign1);

                obMethodInfo.env->DeleteLocalRef(partnerId1);
                obMethodInfo.env->DeleteLocalRef(prepayId1);
                obMethodInfo.env->DeleteLocalRef(nonceStr1);
                obMethodInfo.env->DeleteLocalRef(timeStamp1);
                obMethodInfo.env->DeleteLocalRef(package1);
                obMethodInfo.env->DeleteLocalRef(sign1);
                obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
            }
        }break;
        default:
            break;
    }
}

void PlatformFunction::channelSdkQuit()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo obMethodInfo;
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       "com/duole/sdk/channel/ChannelSdkUtil",
                                       "doSdkQuit",
                                       "()V"))
    {
        obMethodInfo.env->CallStaticVoidMethod(obMethodInfo.classID,
                                               obMethodInfo.methodID);
        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
#endif
}


void PlatformFunction::exitGame()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo obMethodInfo;
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       CLASS_NAME,
                                       "exitGame",
                                       "()V"))
    {
        obMethodInfo.env->CallStaticVoidMethod(obMethodInfo.classID,
                                               obMethodInfo.methodID);
        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
#endif
}


void PlatformFunction::channelSdkLogin()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo obMethodInfo;
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       "com/duole/sdk/channel/ChannelSdkUtil",
                                       "doSdkLogin",
                                       "()V"))
    {
        obMethodInfo.env->CallStaticVoidMethod(obMethodInfo.classID,
                                               obMethodInfo.methodID);
        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
#endif
}

void PlatformFunction::channelGetVerifiedInfo()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo obMethodInfo;
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       "com/duole/sdk/channel/ChannelSdkUtil",
                                       "doGetVerifiedInfo",
                                       "()V"))
    {
        obMethodInfo.env->CallStaticVoidMethod(obMethodInfo.classID,
                                               obMethodInfo.methodID);
        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
#endif
}

//渠道sdk切换账号
void PlatformFunction::channelSdkSwitchAccount()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo obMethodInfo;
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       "com/duole/sdk/channel/ChannelSdkUtil",
                                       "doSdkSwitchAccount",
                                       "()V"))
    {
        obMethodInfo.env->CallStaticVoidMethod(obMethodInfo.classID,
                                               obMethodInfo.methodID);
        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
#endif 
}

string PlatformFunction::getCopyText()
{
    string sRet = "";
    JniMethodInfo obMethodInfo;
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       CLASS_NAME,
                                       "getCopyText",
                                       "()Ljava/lang/String;"))
    {
        jstring jstr = static_cast<jstring>(obMethodInfo.env->CallStaticObjectMethod(obMethodInfo.classID, obMethodInfo.methodID));
        
        if(jstr)
        {
            const char* pszChars = obMethodInfo.env->GetStringUTFChars(jstr, nullptr);
            sRet.assign(pszChars);
            obMethodInfo.env->ReleaseStringUTFChars(jstr, pszChars);
        }
        
        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
    return sRet;
}

//渠道sdk上传玩家信息
void PlatformFunction::upSendUserInfo(std::map<std::string, std::string> orderInfo, int type)
{
    
}

void PlatformFunction::removeAllLocalNotification()
{
    JniMethodInfo obMethodInfo;
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       CLASS_NAME,
                                       "removeAllLocalNotification",
                                       "()V"))
    {
        obMethodInfo.env->CallStaticVoidMethod(obMethodInfo.classID,
                                               obMethodInfo.methodID);
        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
}

void PlatformFunction::addLocalNotification(const string &sText,
                             const string &sTitle,
                             const long &nDelay,
                             const string &sKey)
{
    JniMethodInfo obMethodInfo;
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       CLASS_NAME,
                                       "addLocalNotification",
                                       "(Ljava/lang/String;Ljava/lang/String;JLjava/lang/String;)V"))
    {
      jstring text = obMethodInfo.env->NewStringUTF(sText.c_str());
      jstring title = obMethodInfo.env->NewStringUTF(sTitle.c_str());
      jlong delay = (jlong)nDelay;
      jstring key = obMethodInfo.env->NewStringUTF(sKey.c_str());

        obMethodInfo.env->CallStaticVoidMethod(obMethodInfo.classID,
                                               obMethodInfo.methodID,
                                               text,
                                               title,
                                               delay,
                                               key);

        obMethodInfo.env->DeleteLocalRef(text);
        obMethodInfo.env->DeleteLocalRef(title);
        obMethodInfo.env->DeleteLocalRef(key);
        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
}

extern "C"
{
  JNIEXPORT void JNICALL Java_com_duole_jniutil_PlatformFunction_onAuthFinish(JNIEnv* env, jobject thiz, jstring accessToken, jstring refreshToken, jstring openId, jboolean error)
  {
    std::string sAccessToken = JniHelper::jstring2string(accessToken);
    std::string sRefreshToken = JniHelper::jstring2string(refreshToken);
    std::string sOpenId = JniHelper::jstring2string(openId);
    bool bError = error;
    ValueMap obData;
    obData.insert(make_pair("openid", Value(sOpenId)));
    obData.insert(make_pair("accesstoken", Value(sAccessToken)));
    obData.insert(make_pair("refreshtoken", Value(sRefreshToken)));
    EventDispatcher *pDispatcher = Director::getInstance()->getEventDispatcher();
    pDispatcher->dispatchCustomEvent(s_sEventAuthFinish, &obData);
    s_sEventAuthFinish.clear();
  }

	JNIEXPORT jstring JNICALL Java_com_duole_jniutil_PlatformFunction_getUmengAppKey(JNIEnv* env, jobject thiz)
	{
		return env->NewStringUTF(UMENG_APP_KEY);
    }

  JNIEXPORT jstring JNICALL Java_com_duole_jniutil_PlatformFunction_getWechatAppId(JNIEnv* env, jobject thiz)
  {
    return env->NewStringUTF(WECHAT_APP_ID);
  }

  JNIEXPORT jstring JNICALL Java_com_duole_jniutil_PlatformFunction_getWechatAppSecret(JNIEnv* env, jobject thiz)
  {
    return env->NewStringUTF(WECHAT_APP_SECRETE);
  }

    JNIEXPORT jstring JNICALL Java_com_duole_jniutil_PlatformFunction_getWechatMiniId(JNIEnv* env, jobject thiz)
  {
    return env->NewStringUTF(WECHAT_MINI_APP_ID);
  }

  JNIEXPORT jstring JNICALL Java_com_duole_jniutil_PlatformFunction_getQQAppId(JNIEnv* env, jobject thiz)
  {
    return env->NewStringUTF(QQ_APP_ID);
  }

  JNIEXPORT jstring JNICALL Java_com_duole_jniutil_PlatformFunction_getQQAppSecret(JNIEnv* env, jobject thiz)
  {
    return env->NewStringUTF(QQ_APP_SECRETE);
  }

  JNIEXPORT jstring JNICALL Java_com_duole_jniutil_PlatformFunction_getUdeskDomain(JNIEnv* env, jobject thiz)
  {
    return env->NewStringUTF(UDESK_DOMAIN);
  }

  JNIEXPORT jstring JNICALL Java_com_duole_jniutil_PlatformFunction_getUdeskAppId(JNIEnv* env, jobject thiz)
  {
    return env->NewStringUTF(UDESK_APP_ID);
  }

  JNIEXPORT jstring JNICALL Java_com_duole_jniutil_PlatformFunction_getUdeskAppKey(JNIEnv* env, jobject thiz)
  {
    return env->NewStringUTF(UDESK_APP_KEY);
  }
    
  JNIEXPORT jstring JNICALL Java_com_duole_jniutil_PlatformFunction_getChannelPackage(JNIEnv* env, jobject thiz)
  {
      return env->NewStringUTF(ANDROID_CHANNEL_PACKAGE);
  }

  JNIEXPORT jstring JNICALL Java_com_duole_jniutil_PlatformFunction_getServerHost(JNIEnv* env, jobject thiz)
  {
    return env->NewStringUTF(developer::getServerHost().c_str());
  }
  JNIEXPORT jstring JNICALL Java_com_duole_jniutil_PlatformFunction_getReportEventVersion(JNIEnv* env, jobject thiz)
  {
    return env->NewStringUTF(REPORT_EVENT_VERSION);
  }
  JNIEXPORT jint JNICALL Java_com_duole_paohuzihd_AppActivity_getSceneDirectionKey(JNIEnv* env, jobject thiz)
  {
      int nDir = util::PHZReaderWriter::LoadIntegerFromXML("KEY_SCENE_DIRECTION", 1);
      return nDir;
  }
	//检测更新结果
	JNIEXPORT void JNICALL Java_com_duole_jniutil_CheckUpdate_onCheckVersionUpdateResult(JNIEnv* env, jobject thiz, jstring sresultText, jstring updateLog)
	{
		std::string sresultText1 = JniHelper::jstring2string(sresultText);
		std::string updateLog1 = JniHelper::jstring2string(updateLog);
		app->m_pCheckUpdate->setUpdateInfo(sresultText1, updateLog1);
	}
    
  JNIEXPORT void JNICALL Java_com_duole_jniutil_PlatformFunction_onShareResult(JNIEnv* env, jobject thiz, jboolean success, jint taskId)
  {
      EventDispatcher *pDispatcher = Director::getInstance()->getEventDispatcher();
      int nTaskId = (int)taskId;
      bool bSuccess = success;
      ValueMap obData;
      obData.insert(make_pair("success", Value(bSuccess)));
      obData.insert(make_pair("taskid", Value(nTaskId)));
      obData.insert(make_pair("event", Value(s_sEventOnFinish)));
      log("-------%s",s_sEventOnFinish.c_str());
      pDispatcher->dispatchCustomEvent("EVT_SHARE_RESULT", &obData);
  }
  JNIEXPORT void JNICALL Java_com_duole_sdk_wechat_WeChatSdkUtil_onPayComplete(JNIEnv* env, jobject thiz)
  {
      Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("EVT_COMMON_CHECK_DELIVER");
  }

  JNIEXPORT void JNICALL Java_com_duole_sdk_channel_ChannelSdkUtil_onPayComplete(JNIEnv* env, jobject thiz)
  {
      Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("EVT_COMMON_CHECK_DELIVER");
  }

  JNIEXPORT void JNICALL Java_com_duole_sdk_channel_ChannelSdkUtil_onLoginComplete(JNIEnv *env,
                                                                              jclass cls,
                                                                              jstring token,
                                                                              jstring ssoid)
  {
      string sToken = JniHelper::jstring2string(token);
      string sSsoId = JniHelper::jstring2string(ssoid);
      
      ValueMap obData;
      obData.insert(make_pair("token", Value(sToken)));
      obData.insert(make_pair("ssoid", Value(sSsoId)));
      Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(EVT_CHSNNEL_LOGIN, &obData);
  }

  JNIEXPORT void JNICALL Java_com_duole_sdk_channel_ChannelSdkUtil_onLoginError(JNIEnv *env,
                                                                              jclass cls,
                                                                              jstring loginError)
  {
      string sToken = JniHelper::jstring2string(loginError);
      Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(EVT_CHSNNEL_LOGINERROR, &sToken);
  }

  JNIEXPORT void JNICALL Java_com_duole_sdk_channel_ChannelSdkUtil_onVerifiedResult(JNIEnv *env,jclass cls,
                                                                              jint token)
  {
      CCLOG("HAHAHAHAHAHAHHA");
      int sToken = int(token);
      Value oData(sToken);
      Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(EVT_VERIFIED_RESULT, &oData);
  }

  JNIEXPORT void JNICALL Java_com_duole_jniutil_PlatformFunction_screenShotComplete(JNIEnv* env, jobject thiz)
  {
      Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("NTF_SCREENSHOT_DIALOGS");
  }

  JNIEXPORT void JNICALL Java_com_duole_paohuzihd_AppActivity_onScreenChangeEnd(JNIEnv* env, jobject thiz, jboolean bIsPortrait)
  {
      bool bValue = bIsPortrait;
      Value oData(bValue);
      Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("NTF_SCREEN_DIR_CHANGE_COMPLETE", &oData);
  }

  JNIEXPORT void JNICALL Java_com_duole_paohuzihd_AppActivity_onPromptChange(JNIEnv* env, jobject thi)
  {
      Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("NTF_PROMPT_SCREEN_CHANGE");
  }

  JNIEXPORT void JNICALL Java_com_duole_paohuzihd_AppActivity_onX5OpenShop(JNIEnv* env, jobject thi)
  {
      Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("NTF_X5_OPEN_SHOP");
  }

  JNIEXPORT void JNICALL Java_com_duole_paohuzihd_PermissionsUtil_onShowPermissTip(JNIEnv* env, jclass cls, jstring des, jint type)
  {
      string sDes = JniHelper::jstring2string(des);
      int nType = int(type);
      ValueMap obData;
      obData.insert(make_pair("sDes", Value(sDes)));
      obData.insert(make_pair("nType", Value(nType)));
      Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("NTF_SHOW_PERMISSTIP", &obData);
  }

  JNIEXPORT void JNICALL Java_com_duole_jniutil_PlatformFunction_onSaveSchemesData(JNIEnv* env, jobject thiz, jstring path, jstring data)
  {
    jboolean jresult = true;
    const char* pPath = env->GetStringUTFChars(path, &jresult);
    const char* pData = env->GetStringUTFChars(data,  &jresult);
    PlatformFunction::onSaveSchemesData(pPath, pData);
  }
}

bool PlatformFunction::isEmulator()
{
    jboolean bRet = false;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo obMethodInfo;
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       CLASS_NAME,
                                       "isEmulator",
                                       "()Z"))
    {
        bRet = obMethodInfo.env->CallStaticBooleanMethod(obMethodInfo.classID,
                                                         obMethodInfo.methodID);
        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
#endif
    return bRet == JNI_TRUE ? true : false;
}

/**
 设备震动相关
 */
void PlatformFunction::shake()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo obMethodInfo;
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       CLASS_NAME,
                                       "shake",
                                       "()V"))
    {
        obMethodInfo.env->CallStaticVoidMethod(obMethodInfo.classID,
                                               obMethodInfo.methodID);
        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
#endif
}

// 注册截图通知
void PlatformFunction::addScreenshotNotification()
{
  
}

string PlatformFunction::getSaveWritePath()
{
    JniMethodInfo obMethodInfo;
    string ret = "";
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       CLASS_NAME,
                                       "getSaveWritePath",
                                       "()Ljava/lang/String;"))
    {
        jstring retFromJava = (jstring)obMethodInfo.env->CallStaticObjectMethod(obMethodInfo.classID, 
                                                              obMethodInfo.methodID);
        const char* str = obMethodInfo.env->GetStringUTFChars(retFromJava, 0);
        ret = str;
        obMethodInfo.env->ReleaseStringUTFChars(retFromJava, str);
        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);        
    }
    return ret + "/";
}

string PlatformFunction::getSaveSpritePath()
{
    JniMethodInfo obMethodInfo;
    string ret = "";
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       CLASS_NAME,
                                       "getSaveSpritePath",
                                       "()Ljava/lang/String;"))
    {
        jstring retFromJava = (jstring)obMethodInfo.env->CallStaticObjectMethod(obMethodInfo.classID, 
                                                              obMethodInfo.methodID);
        const char* str = obMethodInfo.env->GetStringUTFChars(retFromJava, 0);
        ret = str;
        obMethodInfo.env->ReleaseStringUTFChars(retFromJava, str);
        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);        
    }
    return ret + "/";
}

// 获取是否有刘海
bool PlatformFunction::hasNotch()
{
    jboolean bRet = false;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo obMethodInfo;
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       CLASS_NAME,
                                       "hasNotch",
                                       "()Z"))
    {
        bRet = obMethodInfo.env->CallStaticBooleanMethod(obMethodInfo.classID,
                                                         obMethodInfo.methodID);
        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
#endif
    return bRet == JNI_TRUE ? true : false;
}

void PlatformFunction::onSaveImage(const std::string &sPath)
{
    JniMethodInfo obMethodInfo;
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       CLASS_NAME,
                                       "onSaveImage",
                                       "(Ljava/lang/String;)V"))
    {
        jstring path = obMethodInfo.env->NewStringUTF(sPath.c_str());
        
        obMethodInfo.env->CallStaticVoidMethod(obMethodInfo.classID,
                                               obMethodInfo.methodID,
                                               path);
        
        obMethodInfo.env->DeleteLocalRef(path);
        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
}

void PlatformFunction::doWeChatAuth(const std::string& sAuthEvent)
{
  s_sEventAuthFinish = sAuthEvent;
    JniMethodInfo obMethodInfo;
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       CLASS_NAME,
                                       "doWeChatAuth",
                                       "()V"))
    {
        obMethodInfo.env->CallStaticVoidMethod(obMethodInfo.classID,
                                               obMethodInfo.methodID);

        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
}

bool PlatformFunction::isMusicActive()
{
    JniMethodInfo obMethodInfo;
    jboolean bRet = false;
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       CLASS_NAME,
                                       "isMusicActive",
                                       "()Z"))
    {
        bRet = obMethodInfo.env->CallStaticBooleanMethod(obMethodInfo.classID,
                                                         obMethodInfo.methodID);

        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
    return bRet == JNI_TRUE ? true : false;
}
// 获取启动耗时(毫秒)
long PlatformFunction::getTimeMillisAppStarted()
{
    JniMethodInfo obMethodInfo;
    jint bRet = 0;
    if (JniHelper::getStaticMethodInfo(obMethodInfo, APP_ACTIVITY, "getTimeMillisAppStarted", "()I")){
        bRet = obMethodInfo.env->CallStaticIntMethod(obMethodInfo.classID, obMethodInfo.methodID);
        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
    return bRet;
}

// 事件上报
void PlatformFunction::doReportPageEvent(const string& sPagename, const std::string& sPlayType, const std::string& sScreenDir, const std::string& sResult)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    log("事件上报%s",sPagename.c_str());
    JniMethodInfo obMethodInfo;
    string device = app->m_pWeb->getDeviceId().c_str();
    string userId = util::PHZReaderWriter::LoadStringFromXML("KEY_LOCAL_USERID", "");
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       SYS_UTIL,
                                       "reportPageEvent", 
                                       "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V"))
    {
        jstring cPagename = obMethodInfo.env->NewStringUTF(sPagename.c_str());
        jstring cPlayType = obMethodInfo.env->NewStringUTF(sPlayType.c_str());
        jstring cScreenDir = obMethodInfo.env->NewStringUTF(sScreenDir.c_str());
        jstring cResult = obMethodInfo.env->NewStringUTF(sResult.c_str());
        jstring device1 = obMethodInfo.env->NewStringUTF(device.c_str());
        jstring userId1 = obMethodInfo.env->NewStringUTF(userId.c_str());

        obMethodInfo.env->CallStaticVoidMethod(obMethodInfo.classID,
                                               obMethodInfo.methodID,
                                               cPagename,
                                               cPlayType,
                                               cScreenDir,
                                               cResult,
                                               device1,
                                               userId1);

        obMethodInfo.env->DeleteLocalRef(cPagename);
        obMethodInfo.env->DeleteLocalRef(cPlayType);
        obMethodInfo.env->DeleteLocalRef(cScreenDir);
        obMethodInfo.env->DeleteLocalRef(cResult);
        obMethodInfo.env->DeleteLocalRef(device1);
        obMethodInfo.env->DeleteLocalRef(userId1);
        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
#endif
}

void PlatformFunction::doReportClickEvent(const string& sEvtName)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    log("点击事件上报%s",sEvtName.c_str());
    JniMethodInfo obMethodInfo;
    string device = app->m_pWeb->getDeviceId().c_str();
    string userId = util::PHZReaderWriter::LoadStringFromXML("KEY_LOCAL_USERID", "");
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       SYS_UTIL,
                                       "reportClickEvent", 
                                       "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V"))
    {
        jstring cEvtname = obMethodInfo.env->NewStringUTF(sEvtName.c_str());
        jstring device1 = obMethodInfo.env->NewStringUTF(device.c_str());
        jstring userId1 = obMethodInfo.env->NewStringUTF(userId.c_str());

        obMethodInfo.env->CallStaticVoidMethod(obMethodInfo.classID,
                                               obMethodInfo.methodID,
                                               cEvtname,
                                               device1,
                                               userId1);

        obMethodInfo.env->DeleteLocalRef(cEvtname);
        obMethodInfo.env->DeleteLocalRef(device1);
        obMethodInfo.env->DeleteLocalRef(userId1);
        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
#endif
}

void PlatformFunction::doScreenChange(int nDir)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
  JniMethodInfo obMethodInfo;
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       APP_ACTIVITY,
                                       "doScreenChange", 
                                       "(I)V"))
    {
        jint nDirection = (jint)nDir;

        obMethodInfo.env->CallStaticVoidMethod(obMethodInfo.classID,
                                               obMethodInfo.methodID,
                                               nDirection
                                               );

        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
#endif
}

int PlatformFunction::getScreenDir()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo obMethodInfo;
    jint bRet = 0;
    if (JniHelper::getStaticMethodInfo(obMethodInfo, APP_ACTIVITY, "getScreenDir", "()I")){
        bRet = obMethodInfo.env->CallStaticIntMethod(obMethodInfo.classID, obMethodInfo.methodID);
        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
    return bRet;
#endif
}

void PlatformFunction::onSaveSchemesData(const std::string &sPath, const std::string& sData)
{
	m_sSchemePath = sPath;
	m_sSchemeData = sData;
}

string PlatformFunction::getSchemePath()
{
    return m_sSchemePath;
}

string PlatformFunction::getSchemeData()
{
    return m_sSchemeData;
}

void PlatformFunction::doAppleLogin()
{
}
// 判断应用存储权限是否需要申请
bool PlatformFunction::checkStoragePermissions()
{
    JniMethodInfo obMethodInfo;
    jboolean bRet = false;
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       PERMISSIONS_TIIL,
                                       "checkStoragePermissions",
                                       "()Z"))
    {
        bRet = obMethodInfo.env->CallStaticBooleanMethod(obMethodInfo.classID,
                                                         obMethodInfo.methodID);

        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
    return bRet == JNI_TRUE ? true : false;
}
// 请求存储权限
void PlatformFunction::requestStoragePermissions()
{
    JniMethodInfo obMethodInfo;
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       PERMISSIONS_TIIL,
                                       "requestStoragePermissions", 
                                       "()V"))
    {
        obMethodInfo.env->CallStaticVoidMethod(obMethodInfo.classID,
                                               obMethodInfo.methodID);

        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
}
void PlatformFunction::setPermissionsCanShowTip(bool canShow)
{
    JniMethodInfo obMethodInfo;
    jboolean bRet = canShow;
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       PERMISSIONS_TIIL,
                                       "setPermissionsCanShowTip", 
                                       "(Z)V"))
    {
        obMethodInfo.env->CallStaticVoidMethod(obMethodInfo.classID,
                                               obMethodInfo.methodID,
                                               bRet);

        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
}

void PlatformFunction::removeLaunchShadeImage()
{
    
}