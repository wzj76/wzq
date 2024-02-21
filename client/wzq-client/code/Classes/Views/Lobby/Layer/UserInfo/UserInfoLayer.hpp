//
//  UserInfo.hpp
//  wzqgame-mobile
//
//  Created by duole on 2024/1/24.
//

#ifndef UserInfo_hpp
#define UserInfo_hpp

#include "cocos2d.h"
#include "AppRes.h"
#include "ui/CocosGUI.h"
#include "cocos-ext.h"
#include "Event/WZQEventWrapper.h"
#include<string>

USING_NS_CC;
USING_NS_CC_EXT;
class UserInfoLayer : public cocos2d::Layer,WZQEventWrapper
{
public:
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(UserInfoLayer);
private:
    cocos2d::Vector<cocos2d::MenuItemSprite*> m_oMenuVector;
    void onMenuCancel(Ref* pSender);
    void onMenuSubmit(Ref* pSender);
    void registSuccess(cocos2d::EventCustom* pEvent);
    int m_HeadNum;
    cocos2d::Sprite* m_head_bg = cocos2d::Sprite::create(HEAD1);  //头像需要更改因此作为成员函数
    Menu* m_pMenu;
    
public:
    void onMenuEsc(Ref* pSender);
    void EditInfo(Ref* pSender);
    void EditHead(Ref* pSender,int head_num);
    void ChooseDone(cocos2d::EventCustom* pEvent);
    void NotChoose(cocos2d::EventCustom* pEvent);
    void ExitLayer(cocos2d::EventCustom* pEvent);
    UserInfoLayer();
};
#endif /* UserInfo_hpp */
