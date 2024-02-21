//
//  AddFriendLayer.hpp
//  wzqgame-mobile
//
//  Created by moe on 2024/2/19.
//

#ifndef AddFriendLayer_hpp
#define AddFriendLayer_hpp

#include "cocos2d.h"
#include "WZQEventWrapper.h"
#include "ui/CocosGUI.h"
#include <vector>

USING_NS_CC;

class AddFriendLayer : public Layer, WZQEventWrapper
{
public:
    AddFriendLayer(){}
    ~AddFriendLayer(){}
    
    CREATE_FUNC(AddFriendLayer);
    
    virtual bool init();
    virtual void onEnter();
    
    void onFriendListClicked(Ref* pSender);
    void onAddFriendClicked(Ref* pSender);
    void onWaitFriendClicked(Ref* pSender);
    
    void AddSuccess(EventCustom* pEvent);
    void IsFriend(EventCustom* pEvent);
    void NotExist(EventCustom* pEvent);
    void onConfirmClicked(Ref* pSender);
    
    void updateFriendInfo(EventCustom* pEvent);
    
private:
    Sprite* m_pBg;
    Menu* m_pMenu;
    std::vector<Node*> m_friendInfo;
    cocos2d::ui::EditBox* m_Editbox;
};

#endif /* AddFriendLayer_hpp */
