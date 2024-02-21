//
//  SigninLayer.hpp
//  wzqgame-mobile
//
//  Created by duole on 2024/1/19.
//

#ifndef SigninLayer_h
#define SigninLayer_h
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
    
    CREATE_FUNC(SigninLayer);
    
private:
    void onMenuCancel(Ref* pSender);
    void onMenuSubmit(Ref* pSender);
public:
    std::string username;
    std::string password;
    std::string value[2];
};

#endif /* RegistLayer_h */

