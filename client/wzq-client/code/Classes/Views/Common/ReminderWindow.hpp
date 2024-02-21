//
//  ReminderWindow.hpp
//  wzqgame-mobile
//
//  Created by moe on 2024/1/19.
//

#ifndef ReminderWindow_hpp
#define ReminderWindow_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "AppRes.h"
#include <string>
#include "ui/CocosGUI.h"
#include "Event/WZQEventWrapper.h"
USING_NS_CC;

class ReminderWindow : public cocos2d::Layer
{
private:
    void buttonCallBack(Ref* pSender);
    
    Ref* m_callbackListener;
    SEL_CallFuncN m_callback;
    
    
public:
    ReminderWindow();
    ~ReminderWindow();
    virtual bool init();
    
    // 静态创建函数
    static ReminderWindow* Create(const std::string backgroundImage, const std::string title, const std::string content);
    
    // 设置标题
    void setTitle(const std::string title, const std::string font = "黑体",int fontsize = 20);
    
    // 设置文本内容
    void setContent(const std::string title, const std::string font = "黑体",int fontsize = 20);
    
    // 设置上层对象和上层回调函数
    void setCallBackFunc(Ref* target, SEL_CallFuncN callFunc);
    
    // 添加按钮
    bool addButton(const std::string btnImage, const std::string btnText);
    
    virtual void onEnter();
    virtual void onExit();
    
    CREATE_FUNC(ReminderWindow);
    
     
    
    
    
    
    
    
};




#endif /* ReminderWindow_hpp */
