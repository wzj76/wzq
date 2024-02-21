//
//  RegistLayer.h
//  phzgame
//
//  Created by admin on 2022/11/28.
//

#ifndef RegistLayer_h
#define RegistLayer_h
#include "cocos2d.h"
#include "AppRes.h"
#include "ui/CocosGUI.h"
#include "cocos-ext.h"
#include <string>
USING_NS_CC_EXT;
#include "Event/WZQEventWrapper.h"
#include"CCLuaEngine.h"

class RegistLayer : public cocos2d::Layer,EditBoxDelegate,WZQEventWrapper
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
private:
    void onMenuCancel(Ref* pSender);
    void onMenuSubmit(Ref* pSender);
    void registSuccess(cocos2d::EventCustom* pEvent);
public:
    std::string username;
    std::string password;
    std::string copyword;
    std::string value[3];
};

#endif /* RegistLayer_h */
