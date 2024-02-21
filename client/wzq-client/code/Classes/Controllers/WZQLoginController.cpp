//
//  LoginController.cpp
//  GouJi
//
//  Created by 曾琪 on 14-9-28.
//
//

#include "WZQLoginController.h"
#include "AppDelegate.h"
#include "Core/AppEvent.h"
#include "Core/AppVersion.h"
#include "Core/AppModel.h"
#include"AppEvent.h"
#include <iostream>
#include <vector>



USING_NS_CC;
using namespace std;

WZQLoginController::WZQLoginController()
:
WZQController("WZQLoginController")
{
}

void WZQLoginController::listEventNames()
{
    m_oEventNames =
    {
        EVT_REGISTER,
        ACK_REGISTER,
        EVT_LOGIN,
        ACK_LOGIN,
    };
}

void WZQLoginController::onEventData(const string &sName, void *pUserData)
{
    if(sName == EVT_REGISTER)
    {
        ValueMap vm = *static_cast<ValueMap*>(pUserData);
        string sName = vm.at("name").asString();
        string password = vm.at("pw").asString();
        doRegistAccount(sName, password);
    }
    else if (sName == ACK_REGISTER)
    {
        
        
        ValueMap oData = *static_cast<ValueMap*>(pUserData);
        int ret = oData.at("resultid").asInt();
        std::string message=oData.at("resultmessage").asString();
        
        if (ret == 0)
        {
            log("注册成功");
            dispatchEvent(REGISTER_SUCCESS);
        }
        else
        {
            log("注册失败 ret = %d", ret);
            cocos2d::MessageBox(message.c_str(), "注册失败");
        }
        
    }
    else if (sName == EVT_LOGIN)
    {
        ValueMap vm = *static_cast<ValueMap*>(pUserData);
        string username = vm.at("name").asString();
        string password = vm.at("pw").asString();
        AppModel::getInstance()->m_pUserModel->setRecordUser(username, password);
        doLogin(username, password);
    }
    else if (sName == ACK_LOGIN)
    {
        ACKUserLogin* pAck = static_cast<ACKUserLogin*>(pUserData);
        //        Value oData = *static_cast<Value*>(pUserData);
        //        int ret = oData.asInt();
        if (pAck)
        {
            int nRet = pAck->result_id();
            if (nRet == 0)
            {
                log("登录成功");
                KKUSERINFO oInfo = {};
                oInfo.sUser = pAck->userinfo().username();
                oInfo.Score = pAck->userinfo().score();
                oInfo.nUserId = pAck->userinfo().id();
                oInfo.Level = pAck->userinfo().level();
                oInfo.Wintimes = pAck->userinfo().wintimes();
                oInfo.Losetimes = pAck->userinfo().losetimes();
                oInfo.Xp = pAck->userinfo().xp();
                oInfo.head=pAck->userinfo().head_id();
                oInfo.coins = pAck->userinfo().coins();
                oInfo.account =pAck->userinfo().account();
                std::cout<<"金币数："<<oInfo.coins<<std::endl;
                
                for(int i = 0; i < pAck->userinfo().props_size();i++)
                {
                    AppModel::getInstance()->m_pShopModel->setItems(pAck->userinfo().props(i).id(), pAck->userinfo().props(i).quantity());
                }
                
                AppModel::getInstance()->m_pUserModel->setMyUserInfo(oInfo);
                dispatchEvent(LOGIN_SUCCESS);
                cout<<"用户头像是: "<<oInfo.head<<endl;
                cout<<"用户"<<oInfo.sUser<<"已登录"<<endl;
                log("登录成功");
            }
            else
            {
                log("登录失败 ret = %d", nRet);
                cocos2d::MessageBox("用户名或密码错误", "登录失败");
            }
        }
    }
    
    
    
}

// 注册
void WZQLoginController::doRegistAccount(const std::string &userName, const std::string &passWord)
{
    app->m_pConnect->SendUserRegister(userName, passWord);
}

// 登录
void WZQLoginController::doLogin(const std::string &sName, const std::string &sPassWord)
{
    app->m_pConnect->SendUserLogin(sName, sPassWord);
}


