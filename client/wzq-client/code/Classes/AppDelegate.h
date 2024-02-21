#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "cocos2d.h"
#include "Net/Socket/WZQConnectProxy.h"
#include "Controllers/WZQControllerManager.h"

/**
@brief    The cocos2d Application.

The reason for implement as private inheritance is to hide some interface call by Director.
*/
class  AppDelegate : private cocos2d::Application
{
public:
    AppDelegate();
    virtual ~AppDelegate();

    virtual void initGLContextAttrs();

    /**
    @brief    Implement Director and Scene init code here.
    @return true    Initialize success, app continue.
    @return false   Initialize failed, app terminate.
    */
    virtual bool applicationDidFinishLaunching();

    /**
    @brief  The function be called when the application enter background
    @param  the pointer of the application
    */
    virtual void applicationDidEnterBackground();

    /**
    @brief  The function be called when the application enter foreground
    @param  the pointer of the application
    */
    virtual void applicationWillEnterForeground();
private:
    void initialize();
    
public:
    WZQConnectProxy*            m_pConnect;            // 连接代理
    WZQControllerManager*       m_pController;         // 控制器管理器
    
private:
   
public:

    bool        m_bInPause;
    long long   m_nPauseTimestamp;
};

extern AppDelegate* app;

#define IS_SCREEN_VERTICAL ((SCREENDIR)PlatformFunction::getScreenDir() == SCREENDIR::SCREEN_VERTICAL && !(appModel->getPlayType() == PLAYTYPE_QIDONG_LIUHUQIANG && appModel->m_pSceneModel->getCurrentScene() == SCENE::kSceneGame))

#endif // _APP_DELEGATE_H_

