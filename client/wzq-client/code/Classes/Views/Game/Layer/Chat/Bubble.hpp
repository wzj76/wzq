//
//  Bubble.hpp
//  wzqgame-mobile
//
//  Created by moe on 2024/2/4.
//

#ifndef Bubble_hpp
#define Bubble_hpp

#include "cocos2d.h"
#include "AppRes.h"
#include "ui/CocosGUI.h"
#include "cocos-ext.h"
#include "Event/WZQEventWrapper.h"
#include "MenuButton.hpp"
#include "WZQWaitingLayer.h"
#include<string>
USING_NS_CC;

USING_NS_CC_EXT;
class Bubble : public cocos2d::Sprite
{
public:
    Bubble();
    ~Bubble();
    
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    static Bubble* create();
    void setText(const std::string text);
    static void pop(Node* parent, const std::string text, Vec2 location, bool isMe = true);
    
private:
    Label* m_pText;
};


#endif /* Bubble_hpp */
