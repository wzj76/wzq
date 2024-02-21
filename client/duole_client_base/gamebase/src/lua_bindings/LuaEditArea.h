//
//  LuaEditArea.h
//  libduole_clientbase iOS
//
//  Created by weiran on 2022/7/15.
//

#ifndef LuaEditArea_h
#define LuaEditArea_h

#include "ui/UIEditArea/UIEditArea.h"
#include "cocos2d/LuaScriptHandlerMgr.h"

namespace duole {
class LuaEditArea : public cocos2d::Ref, public cocos2d::ui::EditAreaDelegate
{
public:
    static LuaEditArea* create(const std::string& sTitle,
                               const std::string& sPlaceholder,
                               const std::string& sText,
                               int nMaxLen,
                               float fTitleHeight,
                               int nTitleFont,
                               bool bLandscape);
    
    virtual bool init(const std::string& sTitle,
                      const std::string& sPlaceholder,
                      const std::string& sText,
                      int nMaxLen,
                      float fTitleHeight,
                      int nTitleFont,
                      bool bLandscape);
    
    void setScriptHandler(cocos2d::ScriptHandlerMgr::HandlerType eHandler);
    cocos2d::ScriptHandlerMgr::HandlerType getScriptHandler();
    std::string getText();
    
private:
    void editAreaEditFinish(const std::string& sText);
    
    LuaEditArea();
    ~LuaEditArea();
    
private:
    cocos2d::ui::EditArea*  m_pEditArea;
    std::string             m_sText;
    cocos2d::ScriptHandlerMgr::HandlerType  m_eScriptHandler;
};
}

#endif /* LuaEditArea_h */
