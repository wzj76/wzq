//
//  LuaImagePicker.hpp
//  libduole_clientbase iOS
//
//  Created by weiran on 2022/7/14.
//

#ifndef LuaImagePicker_h
#define LuaImagePicker_h

#include "ui/UIImagePicker/UIImagePicker.h"
#include "cocos2d/LuaScriptHandlerMgr.h"

namespace duole {
class LuaImagePicker : public cocos2d::Ref, public cocos2d::ui::ImagePickerDelegate
{
public:
    static LuaImagePicker* create(int nRatioX,
                                  int nRatioY,
                                  int nWidth,
                                  int nHeight,
                                  bool bAllowsEditing);
    
    virtual bool init(int nRatioX,
                      int nRatioY,
                      int nWidth,
                      int nHeight,
                      bool bAllowsEditing);
    
    void setScriptHandler(cocos2d::ScriptHandlerMgr::HandlerType eHandler);
    cocos2d::ScriptHandlerMgr::HandlerType getScriptHandler();
    bool isSucceed();
    std::string getFilePath();
    
private:
    void picked(const std::string &sFilePath, bool bSucceed);
    
    LuaImagePicker();
    ~LuaImagePicker();
    
private:
    cocos2d::ui::ImagePicker*   m_pImagePicker;
    std::string                 m_sFilePath;
    bool                        m_bSucceed;
    cocos2d::ScriptHandlerMgr::HandlerType  m_eScriptHandler;
};
}

#endif /* LuaImagePicker_h */
