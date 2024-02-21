//
//  SignLayer.hpp
//  wzqgame-mobile
//
//  Created by duole on 2024/1/19.
//

#ifndef SignLayer_hpp
#define SignLayer_hpp

#include <stdio.h>
#ifndef RegistLayer_h
#define RegistLayer_h
#include "cocos2d.h"
#include "AppRes.h"
#include "ui/CocosGUI.h"
#include "cocos-ext.h"
#include <string>
USING_NS_CC_EXT;
#include "Event/WZQEventWrapper.h"

class SigninLayer : public cocos2d::Layer,EditBoxDelegate,WZQEventWrapper
{
public:
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual void editBoxEditingDidBegin(EditBox* editBox);
    virtual void editBoxEditingDidEnd(EditBox* editBox);
    virtual void editBoxTextChanged(EditBox* editBox, const std::string& text);
    virtual void editBoxReturn(EditBox* editBox);
    
    CREATE_FUNC(RegistLayer);
    
    void onMenuCancel(Ref* pSender);
    void onMenuSubmit(Ref* pSender);
public:
    std::string username;
    std::string password;
    std::string copyword;
    std::string value[3];
    cocos2d::ui::Button *m_pCancelButton;
};
#endif /* SignLayer_hpp */


