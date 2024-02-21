//
//  ReminderWindow.hpp
//  wzqgame-mobile
//
//  Created by moe on 2024/1/19.
//

#ifndef ReminderWindow_hpp
#define ReminderWindow_hpp

#include "cocos2d.h"
#include "AppRes.h"
#include "Core/AppModel.h"
#include "ui/CocosGUI.h"
#include "cocos-ext.h"
#include "Event/WZQEventWrapper.h"
#include "MenuButton.hpp"
#include "WZQWaitingLayer.h"
#include<string>
#include <stdio.h>
#include <vector>
USING_NS_CC;
USING_NS_CC_EXT;
class ReminderWindow : public cocos2d::Layer,WZQEventWrapper,EditBoxDelegate
{
public:
    ReminderWindow();
    ~ReminderWindow();
    
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(ReminderWindow);
    
    virtual void setText(std::string title, std::string content);
    virtual void addButton(MenuButton* button);
    virtual EditBox* addEditBox(std::string placeHoldText = "请输入你想发送的信息:");
    
    virtual void editBoxEditingDidBegin(EditBox* editBox);
    virtual void editBoxEditingDidEnd(EditBox* editBox);
    virtual void editBoxTextChanged(EditBox* editBox, const std::string& text);
    virtual void editBoxReturn(EditBox* editBox);
    
    void initButtons();
    void AddSuccess(EventCustom* pEvent);
    void IsFriend(EventCustom* pEvent);
    void NotExist(EventCustom* pEvent);
    void addCloseButton();
    void delButton(MenuButton* button);
    EditBox* m_Editbox;
private:
    Menu* m_pMenu;
    Label* m_pTitleLable;
    Label* m_pContentLable;
    Sprite* m_pBg;
    int iButtonNum;
    Vector<MenuButton*> m_Buttons;
};

#endif /* ReminderWindow_hpp */
