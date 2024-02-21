//
//  LuaEditArea.cpp
//  libduole_clientbase iOS
//
//  Created by weiran on 2022/7/15.
//

#include "LuaEditArea.h"

using namespace std;
USING_NS_CC;

namespace duole {

LuaEditArea::LuaEditArea()
:
m_pEditArea(nullptr),
m_sText(""),
m_eScriptHandler(ScriptHandlerMgr::HandlerType::NODE)
{}

LuaEditArea::~LuaEditArea()
{
    CC_SAFE_RELEASE_NULL(m_pEditArea);
}

LuaEditArea* LuaEditArea::create(const string& sTitle,
                                 const string& sPlaceholder,
                                 const string& sText,
                                 int nMaxLen,
                                 float fTitleHeight,
                                 int nTitleFont,
                                 bool bLandscape)
{
    LuaEditArea* pRet = new LuaEditArea();
    if (pRet && pRet->init(sTitle, sPlaceholder, sText, nMaxLen, fTitleHeight, nTitleFont, bLandscape))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}

bool LuaEditArea::init(const string& sTitle,
                       const string& sPlaceholder,
                       const string& sText,
                       int nMaxLen,
                       float fTitleHeight,
                       int nTitleFont,
                       bool bLandscape)
{
    m_pEditArea = ui::EditArea::create(sTitle, sPlaceholder, sText, nMaxLen, this, fTitleHeight, nTitleFont, bLandscape);
    m_pEditArea->retain();
    
    return true;
}

void LuaEditArea::setScriptHandler(ScriptHandlerMgr::HandlerType eHandler)
{
    m_eScriptHandler = eHandler;
}

ScriptHandlerMgr::HandlerType LuaEditArea::getScriptHandler()
{
    return m_eScriptHandler;
}

string LuaEditArea::getText()
{
    return m_sText;
}

void LuaEditArea::editAreaEditFinish(const string& sText)
{
    m_sText = sText;
    
    int nHandler = cocos2d::ScriptHandlerMgr::getInstance()->getObjectHandler((void*)this, m_eScriptHandler);
    if (0 != nHandler)
    {
        CommonScriptData data(nHandler, "");
        ScriptEvent event(cocos2d::ScriptEventType::kCommonEvent, (void*)&data);
        ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&event);
    }
}

}
