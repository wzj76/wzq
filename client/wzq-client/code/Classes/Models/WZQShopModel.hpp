//
//  WZQShopModel.hpp
//  wzqgame-mobile
//
//  Created by moe on 2024/2/19.
//

#ifndef WZQShopModel_hpp
#define WZQShopModel_hpp

#include <stdio.h>
#include "WZQModel.h"
#include "Core/AppDef.h"
#include "WZQWaitingLayer.h"
#include <vector>
#include <unordered_map>

class WZQShopModel : public WZQModel
{
public:
    CREATE_MODEL(WZQShopModel);
    WZQShopModel();
    ~WZQShopModel();
    
    void init();
    virtual void reset() override;
    
    void setItems(unsigned itemID, unsigned count);
    
    /*
     * 寻找背包里的物品，找到返回指针
     */
    SHOPITEM* findItem(unsigned id);
    
    /*
     * 寻找背包里的物品，找到返回数量
     */
    unsigned getItemCount(unsigned id);
    
    void setCount(unsigned int id, int count);
public:
    std::vector<std::pair<SHOPITEM, unsigned>> m_items;
    std::vector<SHOPITEM> m_allItems;
    std::unordered_map<std::string, int> m_headList;
    std::unordered_map<std::string, int> m_emojiList;
    std::unordered_map<int, std::string> m_emojiStringList;
    std::unordered_map<std::string, std::string> m_String2EmojiList;

};

#endif /* WZQShopModel_hpp */
