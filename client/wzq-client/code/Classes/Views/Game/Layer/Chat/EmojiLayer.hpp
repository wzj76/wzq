//
//  EmojiLayer.hpp
//  wzqgame-mobile
//
//  Created by moe on 2024/2/21.
//

#ifndef EmojiLayer_hpp
#define EmojiLayer_hpp

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
class EmojiLayer : public cocos2d::Layer,WZQEventWrapper
{
public:
    EmojiLayer();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(EmojiLayer);
private:
    void onMenuCancel(Ref* pSender);
    void onMenuSubmit(Ref* pSender);
    void sendSuccess(cocos2d::EventCustom* pEvent);
    void addScrollViewInfo(int flag);
    void updateScrollView();
    
    
    int m_EmojiNum;
    Sprite* m_pHeadSelect;
    std::vector<MenuButton*> m_EmojiList;
    std::vector<std::string> m_AvailableList;
    
    Sprite* m_pBg;
    Menu* m_pMenu;
    Menu* m_pScrollMenu;
    ui::ScrollView* m_scrollView;
    int m_scrollHeight;
    std::vector<std::pair<Node*, Vec2>> m_scrollContent;
    
    
public:
    void chooseHead(Vec2 pos);
};


#endif /* EmojiLayer_hpp */
