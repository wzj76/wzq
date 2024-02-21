//
//  ChooseHead.hpp
//  wzqgame-mobile
//
//  Created by duole on 2024/1/25.
//

#ifndef ChooseHeadLayer_hpp
#define ChooseHeadLayer_hpp

#include "cocos2d.h"
#include "AppRes.h"
#include "ui/CocosGUI.h"
#include "cocos-ext.h"
#include "Event/WZQEventWrapper.h"
#include "MenuButton.hpp"
#include "AppDef.h"
#include<string>
#include <vector>
USING_NS_CC_EXT;

USING_NS_CC;
class ChooseHeadLayer : public cocos2d::Layer,WZQEventWrapper
{
public:
    ChooseHeadLayer();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(ChooseHeadLayer);
private:
    void onMenuCancel(Ref* pSender);
    void onMenuSubmit(Ref* pSender);
    void registSuccess(cocos2d::EventCustom* pEvent);
    void addScrollViewInfo(int flag);
    void updateScrollView();
    
    
    int m_HeadNum;
    Sprite* m_pHeadSelect;
    std::vector<MenuButton*> m_HeadList;
    std::vector<std::string> m_AvailableList;
    
    Sprite* m_pBg;
    Menu* m_pMenu;
    Menu* m_pScrollMenu;
    ui::ScrollView* m_scrollView;
    int m_scrollHeight;
    std::vector<std::pair<Node*, Vec2>> m_scrollContent;
    
    
public:
//    int ClickHead1(Ref* pSender);
//    int ClickHead2(Ref* pSender);
//    int ClickHead3(Ref* pSender);
//    int ClickHead4(Ref* pSender);
//    int ClickHead5(Ref* pSender);
    void chooseHead(Vec2 pos);
};
#endif /* UserInfo_hpp */
