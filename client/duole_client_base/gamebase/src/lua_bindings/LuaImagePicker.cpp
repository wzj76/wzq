//
//  LuaImagePicker.cpp
//  libduole_clientbase iOS
//
//  Created by weiran on 2022/7/14.
//

#include "LuaImagePicker.h"

using namespace std;
USING_NS_CC;

namespace duole {
LuaImagePicker::LuaImagePicker()
:
m_pImagePicker(nullptr),
m_sFilePath(""),
m_bSucceed(false),
m_eScriptHandler(ScriptHandlerMgr::HandlerType::NODE)
{}

LuaImagePicker::~LuaImagePicker()
{
    CC_SAFE_RELEASE_NULL(m_pImagePicker);
}

LuaImagePicker* LuaImagePicker::create(int nRatioX,
                                       int nRatioY,
                                       int nWidth,
                                       int nHeight,
                                       bool bAllowsEditing)
{
    LuaImagePicker* pRet = new LuaImagePicker();
    if (pRet && pRet->init(nRatioX, nRatioY, nWidth, nHeight, bAllowsEditing))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}

bool LuaImagePicker::init(int nRatioX,
                          int nRatioY,
                          int nWidth,
                          int nHeight,
                          bool bAllowsEditing)
{
    m_pImagePicker = ui::ImagePicker::create(nRatioX, nRatioY, nWidth, nHeight, bAllowsEditing, this);
    m_pImagePicker->retain();
    
    return true;
}

void LuaImagePicker::setScriptHandler(ScriptHandlerMgr::HandlerType eHandler)
{
    m_eScriptHandler = eHandler;
}

ScriptHandlerMgr::HandlerType LuaImagePicker::getScriptHandler()
{
    return m_eScriptHandler;
}

bool LuaImagePicker::isSucceed()
{
    return m_bSucceed;
}

string LuaImagePicker::getFilePath()
{
    return m_sFilePath;
}

void LuaImagePicker::picked(const string &sFilePath, bool bSucceed)
{
    m_sFilePath = sFilePath;
    m_bSucceed = bSucceed;
    
    int nHandler = cocos2d::ScriptHandlerMgr::getInstance()->getObjectHandler((void*)this, m_eScriptHandler);
    if (0 != nHandler)
    {
        CommonScriptData data(nHandler, "");
        ScriptEvent event(cocos2d::ScriptEventType::kCommonEvent, (void*)&data);
        ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&event);
    }
}

}
