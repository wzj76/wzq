//
//  UserInfoLayer.cpp
//  用户个人信息
//
//  Created by wzjqaq on 2024/1/24.
//

#include "UserInfoLayer.hpp"
#include "ChooseHeadLayer.hpp"
#include "Core/AppFunc.h"
#include "Core/AppEvent.h"
#include "Core/AppModel.h"
#include "Controllers/WZQLoginController.h"
#include <iostream>
#include <sstream>
#include "SigninLayer.hpp"
#include "WZQDeviceSize.hpp"
#include "LobbyScene.hpp"
#include "SimpleAudioEngine.h"
#include "MenuButton.hpp"
#include "ReminderWindow.hpp"

#include <iomanip>
USING_NS_CC;

bool UserInfoLayer::init()
{
    if(!Layer::init())
    {
        return false;
    }
    CocosDenshion::SimpleAudioEngine::getInstance() -> preloadEffect(SOUND_BT);
    auto usrinfo=AppModel::getInstance()->m_pUserModel->getMyUserInfo();
    auto bg = Sprite::create(AppModel::getInstance()->m_pUserModel->getThemePath() + BG_POP);
    auto size = bg->getContentSize();
    bg->setAnchorPoint(Vec2(0.5,0));
    addChild(bg,-1);
    auto vsize = Director::getInstance()->getVisibleSize();
    bg->setPosition(Vec2(vsize.width/2,vsize.height*0.15));
    bg -> setScale(WZQDeviceSize::getContentScaleInHeight(bg, 0.7));
    
    //退出按钮
    Sprite* pBtNormalnEsc = Sprite::create(ESC_BT);
    Sprite* pBtnSelectEsc = Sprite::create(ESC_BT);
    pBtnSelectEsc->setColor(Color3B::GRAY);
    
    auto pMenuItemBtnEsc = MenuItemSprite::create(pBtNormalnEsc,pBtnSelectEsc, CC_CALLBACK_1(UserInfoLayer::onMenuEsc, this));
    pMenuItemBtnEsc->setScale(WZQDeviceSize::getContentScaleInHeight(pMenuItemBtnEsc,0.1f));
    WZQDeviceSize::setPosAtNodeRightTop(pMenuItemBtnEsc, bg);
    
    auto pMenuItemBtnSubmit = MenuButton::create("", BT_PENCIL, CC_CALLBACK_1(UserInfoLayer::EditInfo, this));
    pMenuItemBtnSubmit -> setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    pMenuItemBtnSubmit->setScale(WZQDeviceSize::getContentScaleInHeight(pMenuItemBtnSubmit, 0.03f));

    
    int hn=4;
    auto pMenuItemBtn_hSubmit = MenuButton::create("", BT_PENCIL, CC_CALLBACK_1(UserInfoLayer::EditHead, this, hn));
    pMenuItemBtn_hSubmit -> setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    pMenuItemBtn_hSubmit->setScale(WZQDeviceSize::getContentScaleInHeight(pMenuItemBtn_hSubmit, 0.05f));
    
    
    m_pMenu = Menu::create(pMenuItemBtnEsc, pMenuItemBtnSubmit,  pMenuItemBtn_hSubmit,NULL);
    m_pMenu->setPosition(Vec2::ZERO);
    m_pMenu->setAnchorPoint(Vec2::ZERO);
    addChild(m_pMenu);

    
    m_oMenuVector.pushBack(pMenuItemBtnEsc);
    m_oMenuVector.pushBack(pMenuItemBtnSubmit);
    m_oMenuVector.pushBack(pMenuItemBtn_hSubmit);
    //个人信息界面包括：头像 用户名 胜场 总场次 胜率 分数 等级 经验
    //头像
    std::stringstream touxiang;
    int head_num=usrinfo.head;
    printf("jian测试:%d",head_num);
    touxiang<<"touxiang/head"<<head_num<<".png";
    m_head_bg -> setTexture(touxiang.str());
    m_head_bg -> setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    m_head_bg -> setScale(WZQDeviceSize::getContentScaleInHeight(m_head_bg, 0.1));
    addChild(m_head_bg);
    WZQDeviceSize::setPosAtNodeTopMiddle(m_head_bg, bg);
    WZQDeviceSize::setMoveNode(Vec2(0,-bg -> getContentSize().height * bg -> getScaleY() / 30), m_head_bg);
    WZQDeviceSize::setPosAtNodeRightMiddle( pMenuItemBtn_hSubmit,m_head_bg);
    WZQDeviceSize::setMoveNode(Vec2(20,0), pMenuItemBtn_hSubmit);
    
    Vec2 startPos = Vec2(WZQDeviceSize::getViewCenterPos().x - bg -> getContentSize().width * 4 *
                         bg -> getScaleX() / 10,
                         WZQDeviceSize::getViewCenterPos().y + 9 *
                         bg -> getContentSize().height * bg -> getScaleY() / 30);
    
    //用户名
    auto pName_fLable = Label::createWithSystemFont("用户名", "黑体", WZQDeviceSize::getFontSize());
    pName_fLable -> setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    WZQDeviceSize::setPosAtNodeBottomMiddle(pName_fLable, m_head_bg);
    WZQDeviceSize::setMoveNode(Vec2(-bg->getContentSize().width * bg->getScaleX() / 6, -bg -> getContentSize().height * bg -> getScaleY() / 6), pName_fLable);
    addChild(pName_fLable);
    
    auto pNameLable = Label::createWithSystemFont(usrinfo.sUser, "黑体", WZQDeviceSize::getFontSize());
    pNameLable -> setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    WZQDeviceSize::setPosAtNodeBottomMiddle(pNameLable, m_head_bg);
    WZQDeviceSize::setMoveNode(Vec2(bg->getContentSize().width * bg->getScaleX() / 6, -bg -> getContentSize().height * bg -> getScaleY() / 6), pNameLable);
    addChild(pNameLable);
    
    WZQDeviceSize::setPosAtNodeRightMiddle(pMenuItemBtnSubmit, pNameLable);
    WZQDeviceSize::setMoveNode(Vec2(20,0), pMenuItemBtnSubmit);
    
    //胜场
    auto pWin_fLable = Label::createWithSystemFont("胜场", "黑体", WZQDeviceSize::getFontSize());
    pWin_fLable -> setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    WZQDeviceSize::setPosAtNodeBottomMiddle(pWin_fLable, pName_fLable);
    addChild(pWin_fLable);
    
    auto pWinLable = Label::createWithSystemFont(std::to_string(usrinfo.Wintimes), "黑体", WZQDeviceSize::getFontSize());
    pWinLable -> setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    WZQDeviceSize::setPosAtNodeBottomMiddle(pWinLable, pNameLable);
    addChild(pWinLable);
    
    //总场次
    auto pPlayTimes_fLable = Label::createWithSystemFont("总场次", "黑体", WZQDeviceSize::getFontSize());
    pPlayTimes_fLable -> setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    WZQDeviceSize::setPosAtNodeBottomMiddle(pPlayTimes_fLable, pWin_fLable);
    addChild(pPlayTimes_fLable);
    
    auto pPlayTimesLable = Label::createWithSystemFont(std::to_string(usrinfo.Wintimes+usrinfo.Losetimes), "黑体", WZQDeviceSize::getFontSize());
    pPlayTimesLable -> setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    WZQDeviceSize::setPosAtNodeBottomMiddle(pPlayTimesLable, pWinLable);
    addChild(pPlayTimesLable);
    
    //胜率
    auto pWinRate_fLable = Label::createWithSystemFont("胜率", "黑体", WZQDeviceSize::getFontSize());
    pWinRate_fLable -> setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    WZQDeviceSize::setPosAtNodeBottomMiddle(pWinRate_fLable, pPlayTimes_fLable);
    addChild(pWinRate_fLable);
    
    float WinRates=0;
    if((usrinfo.Wintimes+usrinfo.Losetimes) != 0)
    {
        WinRates=(float)usrinfo.Wintimes/(float)(usrinfo.Wintimes+usrinfo.Losetimes);
    }
    std::stringstream sWin;
    sWin << std::fixed << std::setprecision(2) << WinRates*100;
    
    auto pWinRateLable = Label::createWithSystemFont(sWin.str()+"%", "黑体", WZQDeviceSize::getFontSize());
    pWinRateLable -> setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    WZQDeviceSize::setPosAtNodeBottomMiddle(pWinRateLable, pPlayTimesLable);
    addChild(pWinRateLable);
    
    //分数
    auto pScore_fLable = Label::createWithSystemFont("分数", "黑体", WZQDeviceSize::getFontSize());
    pScore_fLable -> setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    WZQDeviceSize::setPosAtNodeBottomMiddle(pScore_fLable, pWinRate_fLable);
    addChild(pScore_fLable);
    
    auto pScoreLable = Label::createWithSystemFont(std::to_string(usrinfo.Score), "黑体", WZQDeviceSize::getFontSize());
    pScoreLable -> setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    WZQDeviceSize::setPosAtNodeBottomMiddle(pScoreLable, pWinRateLable);
    addChild(pScoreLable);
    
    //等级
    auto pLevel_fLable = Label::createWithSystemFont("等级", "黑体", WZQDeviceSize::getFontSize());
    pLevel_fLable -> setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    WZQDeviceSize::setPosAtNodeBottomMiddle(pLevel_fLable, pScore_fLable);
    addChild(pLevel_fLable);
    
    auto pLevelLable = Label::createWithSystemFont(std::to_string(usrinfo.Level), "黑体", WZQDeviceSize::getFontSize());
    pLevelLable -> setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    WZQDeviceSize::setPosAtNodeBottomMiddle(pLevelLable, pScoreLable);
    addChild(pLevelLable);
    
    //经验
    auto pXp_fLable = Label::createWithSystemFont("经验", "黑体", WZQDeviceSize::getFontSize());
    pXp_fLable -> setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    WZQDeviceSize::setPosAtNodeBottomMiddle(pXp_fLable, pLevel_fLable);
    addChild(pXp_fLable);
    
    auto pXpLable = Label::createWithSystemFont(std::to_string(usrinfo.Xp), "黑体", WZQDeviceSize::getFontSize());
    pXpLable -> setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    WZQDeviceSize::setPosAtNodeBottomMiddle(pXpLable, pLevelLable);
    addChild(pXpLable);
    
    WZQDeviceSize::setAnchorNoMove(pMenuItemBtn_hSubmit);
    WZQDeviceSize::setAnchorNoMove(pMenuItemBtnSubmit);
    
    return true;
}

//选完头像
void UserInfoLayer::ChooseDone(cocos2d::EventCustom* pEvent){
    printf("选完了");
    cocos2d::Value* eventData = static_cast<cocos2d::Value*>(pEvent->getUserData());
    int num=eventData->asInt();
    printf("选择头像为:%d",num);
    std::stringstream head_path;
    head_path<<"touxiang/head"<<num<<".png";
    m_head_bg->setTexture(head_path.str());
    //m_head_bg ->setAnchorPoint(Vec2(0.5,0));
    m_head_bg -> setScale(WZQDeviceSize::getContentScaleInHeight(m_head_bg, 0.1));
    for(auto&e:m_oMenuVector)
    {
        e->setVisible(true);
    }
    return;
}

//取消按钮
void UserInfoLayer::NotChoose(cocos2d::EventCustom *pEvent){
    printf("没有选择\n");
    for(auto&e:m_oMenuVector)
    {
        e->setVisible(true);
    }
}
//点击退出按钮回调函数
void UserInfoLayer::onMenuEsc(Ref* pSender){
    dispatchEvent(BUTTONS_BACK);
    dispatchEvent(UPDATE_COINS);
    auto newScene = LobbyScene::createScene();
    Director::getInstance() -> replaceScene(newScene);
    return;
}
//编辑头像
void UserInfoLayer::EditHead(Ref* pSender,int head_num){
    
    for(auto&e:m_oMenuVector)
    {
        e->setVisible(false);
    }
    auto ch = ChooseHeadLayer::create();
    addChild(ch);
    Value oData;
    oData=Value(head_num);
    dispatchEvent(EDIT_HEAD,&oData);
    printf("更换头像:%d\n",head_num);
    return ;
}

//编辑个人信息回调函数
void UserInfoLayer::EditInfo(Ref* pSender){
    printf("更改用户名\n");
    auto layer = ReminderWindow::create();
    auto count = AppModel::getInstance()->m_pShopModel->getItemCount(17);
    layer->setText("修改昵称", "修改昵称将会消耗一次改名卡，\n你现在拥有"+std::to_string(count)+"张改名卡。");
    addChild(layer);
    
    auto pCancelBtn = MenuButton::create("取消", [this, layer](Ref* pSender){
        layer->removeFromParent();
        m_pMenu->setVisible(true);
    });
    
    auto pComfirmBtn = MenuButton::create("确认", [this,layer, count](Ref* pSender){
        log("确定修改用户名。");
        if(count < 1)
        {
            MessageBox("改名卡数量不足，请从商城购买！", "温馨提示");
            return;
        }
        std::string username = layer->m_Editbox->getText();
        if(username.empty()||username.size()>10)
        {
            MessageBox("名字为空或过长！", "温馨提示");
            return;
        }
        //发送修改名字的消息
        Value oData;
        oData=Value(username);
        dispatchEvent(EVT_EDIT_USER_NAME,&oData);
        printf("修改名字:%s\n",username.c_str());
        
        
    });
    m_pMenu->setVisible(false);
    layer->addEditBox("请输入你想要的昵称：");
    layer->addButton(pCancelBtn);
    layer->addButton(pComfirmBtn);
    layer->initButtons();
    
    
}

void UserInfoLayer::onEnter()
{
    
    Layer::onEnter();
    ADD_OBSERVER(UserInfoLayer::ChooseDone, CHOOSE_DONE);
    ADD_OBSERVER(UserInfoLayer::NotChoose, NOT_CHOOSE);
    ADD_OBSERVER(UserInfoLayer::ExitLayer, EDIT_NAME_SUCCESS);
}

void UserInfoLayer::onExit()
{
    Layer::onExit();
}

UserInfoLayer::UserInfoLayer(): m_HeadNum(-1){}

void UserInfoLayer::ExitLayer(cocos2d::EventCustom *pEvent)
{
    int count = AppModel::getInstance()->m_pShopModel->getItemCount(17);
    AppModel::getInstance()->m_pShopModel->setCount(17, count-1);
    dispatchEvent(BUTTONS_BACK);
    dispatchEvent(UPDATE_COINS);
    auto newScene = LobbyScene::createScene();
    Director::getInstance() -> replaceScene(newScene);
    return;
}

