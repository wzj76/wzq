//
//  WZQShopModel.cpp
//  wzqgame-mobile
//
//  Created by moe on 2024/2/19.
//

#include "WZQShopModel.hpp"
#include "AppRes.h"
#include "iostream"

WZQShopModel::WZQShopModel()
:WZQModel("WZQShopModel")
{
    // 全物品一览表
    m_allItems.push_back(SHOPITEM(1, "请吃热狗", "表情", 200, EMOJI_HOTDOG));
    m_allItems.push_back(SHOPITEM(2, "请吃汉堡", "表情", 200, EMOJI_BURGER));
    m_allItems.push_back(SHOPITEM(3, "开心", "表情", 200, EMOJI_HAPPY));
    m_allItems.push_back(SHOPITEM(4, "晕", "表情", 200, EMOJI_DIZZY));
    m_allItems.push_back(SHOPITEM(5, "微笑", "表情", 200, EMOJI_SMILE));
    m_allItems.push_back(SHOPITEM(6, "尊贵头像1", "头像", 1000, HEAD6));
    m_allItems.push_back(SHOPITEM(7, "尊贵头像2", "头像", 1000, HEAD7));
    m_allItems.push_back(SHOPITEM(8, "尊贵头像3", "头像", 1000, HEAD8));
    m_allItems.push_back(SHOPITEM(9, "尊贵头像4", "头像", 1000, HEAD9));
    m_allItems.push_back(SHOPITEM(10, "尊贵头像5", "头像", 1000, HEAD10));
    m_allItems.push_back(SHOPITEM(11, "尊贵头像6", "头像", 1000, HEAD11));
    m_allItems.push_back(SHOPITEM(12, "冰封天蓝", "主题", 2000, PROP_BLUE));
    m_allItems.push_back(SHOPITEM(13, "苍翠深绿", "主题", 2000, PROP_GREEN));
    m_allItems.push_back(SHOPITEM(14, "热情大红", "主题", 2000, PROP_RED));
    m_allItems.push_back(SHOPITEM(15, "夏日暖黄", "主题", 2000, PROP_YELLOW));
    m_allItems.push_back(SHOPITEM(16, "诱惑魅紫", "主题", 2000, PROP_PURPLE));
    m_allItems.push_back(SHOPITEM(17, "改名卡", "道具", 10000, PROP_NAMECHANGE));
    
    // 头像与头像id一览表
    m_headList[HEAD1] = 1;
    m_headList[HEAD2] = 2;
    m_headList[HEAD3] = 3;
    m_headList[HEAD4] = 4;
    m_headList[HEAD5] = 5;
    m_headList[HEAD6] = 6;
    m_headList[HEAD7] = 7;
    m_headList[HEAD8] = 8;
    m_headList[HEAD9] = 9;
    m_headList[HEAD10] = 10;
    m_headList[HEAD11] = 11;
    
    // 表情和表情id一览表
    m_emojiList[EMOJI_HOTDOG] = 1;
    m_emojiList[EMOJI_BURGER] = 2;
    m_emojiList[EMOJI_HAPPY] = 3;
    m_emojiList[EMOJI_DIZZY] = 4;
    m_emojiList[EMOJI_SMILE] = 5;
    
    // 表情id和发送消息一览表
    m_emojiStringList[1] = "[发送了【请吃热狗】]";
    m_emojiStringList[2] = "[发送了【请吃汉堡】]";
    m_emojiStringList[3] = "[发送了【开心】]";
    m_emojiStringList[4] = "[发送了【晕】]";
    m_emojiStringList[5] = "[发送了【微笑】]";
    
    // 发送信息和表情一览表
    m_String2EmojiList["[发送了【请吃热狗】]"] = EMOJI_HOTDOG;
    m_String2EmojiList["[发送了【请吃汉堡】]"] = EMOJI_BURGER;
    m_String2EmojiList["[发送了【开心】]"] = EMOJI_HAPPY;
    m_String2EmojiList["[发送了【晕】]"] = EMOJI_DIZZY;
    m_String2EmojiList["[发送了【微笑】]"] = EMOJI_SMILE;
    
};
WZQShopModel::~WZQShopModel()
{

};
void WZQShopModel::init()
{
}

void WZQShopModel::reset()
{
}

void WZQShopModel::setItems(unsigned itemID, unsigned int count) {
    
    // 先找到有没有这个东西
    for(auto& i : m_items)
    {
        if(itemID == i.first.ID)
        {
            i.second = count;
            return;
        }
    }
    // 没有则插入
    for(auto& i : m_allItems)
    {
        if(itemID == i.ID)
        {
            std::cout<<i.name<<"添加成功，数量："<<count<<std::endl;
            m_items.push_back(std::make_pair(i, count));
            return;
        }
    }
}

SHOPITEM* WZQShopModel::findItem(unsigned id)
{
    for(auto &i : m_items)
    {
        if (i.first.ID == id)
            return &(i.first);
    }
    return nullptr;
}

unsigned int WZQShopModel::getItemCount(unsigned int id)
{
    for(auto &i : m_items)
    {
        if (i.first.ID == id)
            return i.second;
    }
    return 0;
}

void WZQShopModel::setCount(unsigned int id, int count)
{
    for(auto &i : m_items)
    {
        if (i.first.ID == id)
            i.second = count;
    }
}





