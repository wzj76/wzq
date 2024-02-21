//
//  PaoDKGame.h
//  ShengJSvrd
//
//  Created by liuting on 15-10-15.
//  Copyright (c) 2015年 liuting. All rights reserved.
//

#ifndef __ShengJSvrd__ShengJGame__
#define __ShengJSvrd__ShengJGame__

#include <iostream>
#include "RedisIntf.h"
#include "ILog.h"
#include "mcgevent.h"
#include <mutex>
#define BOARD_SIZE 13
#define BLACK_CHESS 1
#define WHITE_CHESS 2
#define OUTTIME_TIME 1000*20
class CTable;
class CWZQGame
{
public :
    CWZQGame(ILog* pLog,CTable* pTable ,int black_id,int white_id);
    ~CWZQGame();
    //初始化
    virtual int OnInit(bool bInitModel){return 0;}
    virtual int OnTimer() ;
	//
    virtual int OnGameStart(){return 0;}
	
    virtual int OnClientMessage(int64 user_id, DWORD dwMsgID, DWORD dwSeqID, const byte* pBuffer, int nLen);
    

    void PutChess(int action_id,int row,int col);
    void UndoPlayerAsk(int action_id);
    void UndoChess(int action_id,bool is_agree);
    int CheckWin(int32 table_id,int32 row,int32 col,int32 color);
    void PlayerForceExitGame(int exit_player_id);
    void Clear();
    
    
private:
    int WriteErrorLog(const char* pszFmt,...);
    int WriteInfoLog(const char* pszFmt,...);
    int WriteWarningLog(const char* pszFmt,...);
    
    int CheckWin(int32 row,int32 col,int32 color);
    bool five(int row ,int col , int row_off , int col_off , int color);
    void GameOver(int32 winner_id,int32 loser_id,int32 table_id,int score,int conis , bool is_force_exit);
    
    //超时自动下棋
    static void* AutoPutChess(void* ptr,unsigned int a);
    static void* AutoRefuseUndoChess(void* ptr,unsigned int a);
public:
    ILog * m_pLog ;
    CTable* m_pTable;
    HANDLE m_gameTimer;
    HANDLE m_undoTimer;
    std::mutex m_mutex;
    
    int m_nBlackPlayerID; //黑白棋的玩家id
    int m_nWhitePlayerID;
    int m_nCurPutChessID; //当前该下棋的玩家id，一开始是黑棋，黑棋下完变为白棋
    
    int m_nBlackPlayerOutTimeCount;
    int m_nWhitePlayerOutTimeCount;
    
    bool m_bIsUndoChcess; //目前这个房间是否正在悔棋，如果正在悔棋则不能进行下棋操作
    int m_remain_time; // 悔棋的剩余时间
    std::vector<std::vector<int>> m_board; //棋盘
    std::vector<std::pair<int,int>>m_blackPlayerActions; //记录黑棋玩家的下棋操作
    std::vector<std::pair<int,int>>m_whitePlayerActions; //记录棋玩家的下棋操作
};

#endif /* defined(__ShengJSvrd__ShengJGame__) */
