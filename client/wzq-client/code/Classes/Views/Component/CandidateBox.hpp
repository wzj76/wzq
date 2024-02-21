//
//  CandidateBox.hpp
//  wzqgame-mobile
//
//  Created by moe on 2024/2/6.
//

#ifndef CandidateBox_hpp
#define CandidateBox_hpp

#include "Core/AppFunc.h"
#include "Core/AppEvent.h"
#include "Core/AppModel.h"
#include "Core/AppRes.h"
#include <string>


class CandidateBox : public cocos2d::Sprite
{
public:
    virtual bool init(const std::string unselected, const std::string selected);
    

    static CandidateBox* create(const std::string unselected = BOX_UNSELECT, const std::string selected = BOX_SELECT);
    

    bool isSelected() const
    {
        return selected_;
    }
    
    void setSelectedCallback(const ccMenuCallback& callback)
    {
        m_callback = callback;
    }
    void setUnselectedCallback(const ccMenuCallback& callback)
    {
        m_unselectedCallback = callback;
    }
    
    void setSelected(bool flag)
    {
        selected_ = flag;
        if(flag)
            setTexture(selectedImage_);
        else
            setTexture(unselectedImage_);
    }
    
    CandidateBox():selected_(false)
    {}

private:
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
    {
        if (this->getBoundingBox().containsPoint(touch->getLocation()))
        {
            if (selected_)
            {
                // 切换到未选中状态
                setTexture(unselectedImage_);
                m_unselectedCallback(this);
                selected_ = false;
            }
            else
            {
                // 切换到选中状态
                setTexture(selectedImage_);
                m_callback(this);
                selected_ = true;
            }
        }

        return true;
    }

    std::string unselectedImage_;
    std::string selectedImage_;
    bool selected_;
    ccMenuCallback m_callback;
    ccMenuCallback m_unselectedCallback;
};
#endif /* CandidateBox_hpp */
