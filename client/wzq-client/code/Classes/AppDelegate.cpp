#include "AppDelegate.h"
#include "AppModel.h"
#include "bugly_v2/plugin/CrashReport.h"
#include "PlatformFunction.h"
#include <iostream>

// #define USE_AUDIO_ENGINE 1
#define USE_SIMPLE_AUDIO_ENGINE 1
#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your game!"
#endif

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif

USING_NS_CC;
using namespace std;

#define KEY_DEVICEID      "DeviceID"

#include "LoginLayer.h"
AppDelegate*    app         = nullptr;

AppDelegate::AppDelegate()
:
m_bInPause(false),
m_nPauseTimestamp(0)
{
    app = this;}

AppDelegate::~AppDelegate()
{
    app = nullptr;
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};
    
    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview)
    {
        glview = GLViewImpl::create("多乐五子棋");
        director->setOpenGLView(glview);
    }

    // turn off display FPS
#ifdef DEBUG_MODE
    //director->setDisplayStats(true);
#endif
    
    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    
    //静态文件优先
    std::string firstPath = StringUtils::format("%supd",FileUtils::getInstance()->getWritablePath().c_str());
    FileUtils::getInstance()->addSearchPath(firstPath,true);
    
    
    PlatformFunction::removeLaunchShadeImage();
    
    // 初始化
    initialize();

    // gitlab test: shop-develop  , 24-01-24
    // gitlab test: shop-develop  , 24-01-25
    
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    CocosDenshion::SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
    
    time_t tNow;
    time(&tNow);
    
    m_bInPause = true;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    m_nPauseTimestamp = (long long)tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    if (m_bInPause)
    {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        long long curTime = (long long)tv.tv_sec * 1000 + tv.tv_usec / 1000;
        long long nResumeTime = curTime - m_nPauseTimestamp;
        if (nResumeTime > 0)
        {
        }
    }
    m_bInPause = false;
}

void AppDelegate::initialize()
{
    
    AppModel::getInstance()->initialize();
    auto Loginscene = LoginLayer::createScene();
    Director::getInstance()->runWithScene(Loginscene);
    
    // 网路初始化
    m_pConnect = new WZQConnectProxy();
    m_pConnect->Configure();
    m_pConnect->DoConnect(); // 启动连接
    
    
    std::cout<<"开始初始化Controller模块"<<std::endl;
    
    // 初始化Controller模块
    m_pController = new WZQControllerManager();
    m_pController->initialize();
    std::cout<<"初始化Controller模块结束"<<std::endl;
}

