#pragma once

#include "cocos2d.h"

#include <string>
#include <vector>
#include "editor-support/cocostudio/CCArmature.h"
#include "MenuButton.hpp"
#include "WZQDeviceSize.hpp"

// 连接服务器信息
typedef struct
{
    std::string     sHost;                          // 服务器地址
    unsigned short  nPort;                          // 端口
} PKCONNINFO, *LPPKCONNINFO;


// 用户信息
typedef struct
{
    unsigned long long  nUserId;                    //  用户id
    std::string         account;                    //  账号（游客用的是设备号
    std::string         sUser;                      //  用户名,可以修改
    int Wintimes;
    int Losetimes;
    int Score;
    int Level;
    int Xp;
    int head;
    unsigned int coins;
    bool isOnline;
} KKUSERINFO, *LPKKUSERINFO;

// 游戏模式枚举
enum GAMEMODE
{
    MatchingMode,   // 匹配模式
    BeatingMode,    // 双人对战模式
    BotMode        // 人机模式
};

// 棋盘大小枚举
enum BOARDSIZE
{
    Size13x13,
    Size15x15,
    Size17x17,
    Size19x19
};

// 主题枚举
enum THEMECONF
{
    green,
    red,
    purple,
    yellow,
    blue
};

// 游戏内信息
typedef struct
{
    GAMEMODE gamemode;
    KKUSERINFO oMyInfo;
    KKUSERINFO oOppoInfo;
    int iRoomId;
    unsigned long long llBlackId;
    unsigned long long llWhiteId;
    unsigned long long llActionId;
    BOARDSIZE boardSize;
    int iWaitingTime;
} INGAMECONF;

//黑棋位置
typedef struct{
    int row;
    int col;
} BLACKPOSITION;
//白棋位置
typedef struct{
    int row;
    int col;
} WHITEPOSITION;
//历史记录
typedef struct{
    KKUSERINFO enemy_userinfo;
    int game_id ;
    int winner_id;
    int loser_id ;
    int score ;
    int black_id ;
    int white_id ;
    bool is_no_winner ;
    bool is_force_exit ;
    std::vector<BLACKPOSITION> black_position;
    std::vector<WHITEPOSITION> white_positon;
    std::string game_time ;
    
} HISTORYINFO;

// 游戏设置
typedef struct
{
    bool background_music;
    bool sound_effect;
    bool button_effect;
    bool chess_effect;
    int theme_color;
} SETUPCONF;

typedef struct Shopitem
{
    std::string name;
    std::string sort;
    int price;
    int ID;
    std::string imgPath;
    Sprite* itemIcon;
    Label* itemName;
    Label* itemSort;
    Label* itemPrice;
    
    Shopitem()
    {
        ID = 0;
        name = "";
        sort = "";
        price = 0;
        imgPath = "";
    };
    Shopitem(int itmID, std::string itmname, std::string itmsort, int itmprice, std::string itmimgPath)
    {
        ID = itmID;
        name = itmname;
        sort = itmsort;
        price = itmprice;
        imgPath = itmimgPath;
    }
    
    bool operator=(Shopitem value)
    {
        if(ID == value.ID)
            return true;
        else
            return false;
    }
} SHOPITEM;
