//
//  ShengJGame.cpp
//  ShengJSvrd
//
//  Created by liuting on 15-10-15.
//  Copyright (c) 2015年 liuting. All rights reserved.
//

#include "WZQGame.h"
#include "GameService.h"
#include <json/json.h>
#include "Table.h"
#include "Player.h"
#include "AutoRobot.hpp"
extern CGameService* g_pService ;

CWZQGame::CWZQGame(ILog* pLog,CTable* pTable ,int black_id,int white_id)
{
    m_pLog = pLog;
    m_pTable =pTable;
    m_nBlackPlayerID = black_id;
    m_nWhitePlayerID = white_id;
    m_nCurPutChessID = black_id;
    m_nBlackPlayerOutTimeCount=0;
    m_nWhitePlayerOutTimeCount=0;
    m_bIsUndoChcess = false;
    m_board.resize(BOARD_SIZE,std::vector<int>(BOARD_SIZE,0));
    m_gameTimer = NULL;
    m_undoTimer = NULL;
    
    m_gameTimer = CMcgTimer::CreateTimer(CWZQGame::AutoPutChess, this, OUTTIME_TIME, OUTTIME_TIME);
}
CWZQGame::~CWZQGame()
{
    if(m_gameTimer!=nullptr)
    {
        CMcgTimer::DeleteTimer(m_gameTimer,true);
        m_gameTimer=nullptr;
    }
    if(m_undoTimer!=nullptr)
    {
        CMcgTimer::DeleteTimer(m_undoTimer,true);
        m_undoTimer=nullptr;
    }
}
int CWZQGame::WriteErrorLog(const char* pszFmt,...)
{
    static char szBuffer[8192];
    va_list args;
    va_start(args, pszFmt);
    int nTmpLen = vsnprintf(szBuffer,8192,pszFmt, args);
    va_end(args);
    
    if (nTmpLen < 0)
    {
        return -1;
    }
    
    printf("Table(ID = %d)::%s \n",m_pTable->GetTableID(),szBuffer);
    m_pLog->Error("Table(ID = %d)::%s",m_pTable->GetTableID(), szBuffer);
    return 0;
}

int CWZQGame::WriteInfoLog(const char* pszFmt,...)
{
    static char szBuffer[8192];
    va_list args;
    va_start(args, pszFmt);
    int nTmpLen = vsnprintf(szBuffer,8192,pszFmt, args);
    va_end(args);
    
    if (nTmpLen < 0)
    {
        return -1;
    }
    
    printf("Table(ID = %d)::%s \n",m_pTable->GetTableID(),szBuffer);
    m_pLog->Info("Table(ID = %d)::%s",m_pTable->GetTableID(), szBuffer);
    return 0;
}

int CWZQGame::WriteWarningLog(const char* pszFmt,...)
{
    static char szBuffer[8192];
    va_list args;
    va_start(args, pszFmt);
    int nTmpLen = vsnprintf(szBuffer,8192,pszFmt, args);
    va_end(args);
    
    if (nTmpLen < 0)
    {
        return -1;
    }
    
    printf("Table(ID = %d)::%s \n",m_pTable->GetTableID(),szBuffer);
    m_pLog->Warning("Table(ID = %d)::%s",m_pTable->GetTableID(), szBuffer);
    return 0;
}

//定时器
int CWZQGame::OnTimer()
{
    return 0;
}

int CWZQGame::OnClientMessage(int64 user_id, DWORD dwMsgID, DWORD dwSeqID, const byte* pBuffer, int nLen)
{
    WriteInfoLog("%s, recv msg(0x%08X) from %lld", __FUNCTION__, dwMsgID,user_id);
    bool bHandle = false;
    
    switch (dwMsgID) {
        default:
            break;
    }
    return bHandle ? 0 : -1;
}


void CWZQGame::PutChess(int action_id,int action_board_row,int action_board_col)
{
    auto& table = m_pTable;
    auto& p_gameService = table -> m_pService;
    auto& p_playerMgr = p_gameService->m_pPlayerMgr;
    if( p_playerMgr->m_playersMap.find(m_nWhitePlayerID)== p_playerMgr->m_playersMap.end())
    {
        std::cout<<" m_pPlayerMgr->m_playersMap找不到 white_id:"<<m_nWhitePlayerID<<std::endl;
        return;
    }

    std::shared_ptr<CPlayer> player1 = p_playerMgr->m_playersMap[m_nWhitePlayerID];
    if( p_playerMgr->m_playersMap.find(m_nBlackPlayerID)== p_playerMgr->m_playersMap.end())
    {
        std::cout<<" m_pPlayerMgr->m_playersMap找不到 black_id:"<<m_nBlackPlayerID<<std::endl;
        return;
    }
    std::shared_ptr<CPlayer>player2 = p_playerMgr->m_playersMap[m_nBlackPlayerID];
    

    int64 client_id1 = player1 -> m_nClientID;
    int64 client_id2 = player2 -> m_nClientID;
    
    std::unique_lock<std::mutex> lock(m_mutex);
    //将自动下棋重置
    CMcgTimer::ChangeTimer(m_gameTimer,CWZQGame::AutoPutChess, this, OUTTIME_TIME, OUTTIME_TIME);
    
    if(m_bIsUndoChcess == true)
    {
        std::cout<<"error!!! 目前该房间正在悔棋，暂时无法进行下棋操作"<<std::endl;
        return;
    }
    
    if(action_id != m_nCurPutChessID)
    {
        std::cout<<"error!!! 还没轮到该用户操作,应该操作的用户:"<<m_nCurPutChessID<<"  实际的用户:"<<action_id<<std::endl;
        return;
    }
    

    int cur_color = action_id == m_nWhitePlayerID ? WHITE_CHESS : BLACK_CHESS ;
    m_board[action_board_row][action_board_col] = cur_color;
    
    //记录操作
    if(cur_color == WHITE_CHESS)
    {
        m_whitePlayerActions.push_back({action_board_row,action_board_col});
    }
    else
    {
        m_blackPlayerActions.push_back({action_board_row,action_board_col});
    }
    m_nCurPutChessID = action_id == m_nWhitePlayerID ? m_nBlackPlayerID : m_nWhitePlayerID;
        
    //下棋结束,将这个下棋的消息广播给两个人
    ACKAction ack;
    ack.set_result_msg("put_chess_sucess");
    ack.set_action_id(action_id);
    ack.set_action_board_row(action_board_row);
    ack.set_action_board_col(action_board_col);
    ack.set_is_win(false);
    //判读是否有玩家胜利
    int winner_id = CheckWin(action_board_row,action_board_col,cur_color);
    if(winner_id != 0)
    {
        ack.set_is_win(true);
        ack.set_win_id(winner_id);
        int loser_id = winner_id == m_nWhitePlayerID ? m_nBlackPlayerID : m_nWhitePlayerID;
        ack.set_lose_id(loser_id);
        
        //胜利者产生，发给auth服务，让它给两个用户加分和减分
        //让auth保存用户对局
        GameOver(winner_id,loser_id,table->m_nTableID,100,200,false);
    }
    // 判断是否平局
    if((m_whitePlayerActions.size() + m_blackPlayerActions.size()) == (m_board[0].size() *  m_board.size()))
    {
        //平局
        ack.set_is_win(true);
        ack.set_win_id(-1);
        ack.set_lose_id(-1);
        GameOver(m_nWhitePlayerID,m_nBlackPlayerID,table->m_nTableID,0,0,false);
    }
    //判断是否有人超时
    if(m_nBlackPlayerOutTimeCount>=5 || m_nWhitePlayerOutTimeCount>=5)
    {
        if(m_nBlackPlayerOutTimeCount>=5)
        {
            winner_id = m_nWhitePlayerID;
            int loser_id = m_nBlackPlayerID;
            ack.set_is_win(true);
            ack.set_win_id(winner_id);
            ack.set_lose_id(loser_id);
            ack.set_win_msg("下棋超时");
            GameOver(winner_id,loser_id,table->m_nTableID,100,200,true);
        }
        else if(m_nWhitePlayerOutTimeCount>=5)
        {
            winner_id = m_nBlackPlayerID;
            int loser_id = m_nWhitePlayerID;
            ack.set_is_win(true);
            ack.set_win_id(winner_id);
            ack.set_lose_id(loser_id);
            ack.set_win_msg("下棋超时");
            GameOver(winner_id,loser_id,table->m_nTableID,100,200,true);
        }
    }
    
    
    std::cout<<"debug---用户进行了一次下棋请求，下棋的用户id:"<<action_id <<" row:"<<action_board_row<<" col:"<<action_board_col<<" 胜利者:"<<winner_id<<std::endl;
    
    p_gameService -> SendSvrdMsg(p_gameService -> GetConnSvrdMsgRoute(), (MSGID_ACTION | ID_ACK), 0, 0, ack.SerializeAsString(), client_id1);
    p_gameService -> SendSvrdMsg(p_gameService -> GetConnSvrdMsgRoute(), (MSGID_ACTION | ID_ACK), 0, 0, ack.SerializeAsString(), client_id2);
    
//    if(table ->m_statu ==room_statu::GAME_OVER)
//    {
//        //移除table和player
//        printf("移除table和player\n");
//        p_gameService->m_pPlayerMgr-> RemovePlayer(table ->m_pGame -> m_nWhitePlayerID);
//        p_gameService->m_pPlayerMgr->RemovePlayer(table ->m_pGame -> m_nBlackPlayerID);
//        p_gameService->m_pTableMgr->RemoveTable(table ->m_nTableID);
//        printf("移除table和player完成\n");
//    }
}

bool CWZQGame::five(int row ,int col , int row_off , int col_off , int color)
{
  int count = 1 ;
  int search_row = row + row_off ;
  int search_col = col + col_off ;
  //检索前一半
  while(search_row >= 0 && search_row < BOARD_SIZE &&
      search_col >=0 && search_col < BOARD_SIZE &&
        m_board[search_row][search_col] == color)
  {
    count++;
    search_row += row_off;
    search_col += col_off;
  }
  
  //检索后一半
  search_row = row - row_off ;
  search_col = col - col_off ;
  while(search_row >= 0 && search_row < BOARD_SIZE &&
      search_col >=0 && search_col < BOARD_SIZE &&
        m_board[search_row][search_col] == color)
  {
    count++;
    search_row -= row_off;
    search_col -= col_off;
  }
  return (count >= 5);
}


int CWZQGame::CheckWin(int32 row,int32 col,int32 color)
{
    //任意一方向达到五子
    if (five(row,col,1,0,color) ||
    five(row,col,0,1,color)  ||
    five(row,col,-1,-1,color) ||
    five(row,col,-1,1,color) )
    {
      int winner_id = color == WHITE_CHESS ? m_nWhitePlayerID  :m_nBlackPlayerID ;
      return winner_id;
    }
    return 0;
}

void CWZQGame::GameOver(int32 winner_id,int32 loser_id,int32 table_id,int score,int conis , bool is_force_exit)
{
    if(m_gameTimer!=nullptr)
    {
        CMcgTimer::DeleteTimer(m_gameTimer,true);
        m_gameTimer=nullptr;
    }
    printf("发给auth服务修改两人的分数\n");
    REQGameOver req;
    req.set_winner_id(winner_id);
    req.set_loser_id(loser_id);
    req.set_score(score);
    req.set_conis(conis);
    
    req.set_black_id(m_nBlackPlayerID);
    req.set_white_id(m_nWhitePlayerID);
    if(score == 0)
    {
        req.set_is_no_winner(true);
    }
    else
    {
        req.set_is_no_winner(false);
    }
    req.set_is_force_exit(is_force_exit);
    
    for (size_t i = 0; i < m_blackPlayerActions.size(); ++i)
    {
        Position *black_position = req.add_black_positions();
        black_position->set_row(m_blackPlayerActions[i].first);
        black_position->set_col(m_blackPlayerActions[i].second);
    }
    for (size_t i = 0; i < m_whitePlayerActions.size(); ++i)
    {
        Position *white_position = req.add_white_positions();
        white_position->set_row(m_whitePlayerActions[i].first);
        white_position->set_col(m_whitePlayerActions[i].second);
    }
    
    auto& table = m_pTable;
    auto& p_gameService = table -> m_pService;
    
    
    p_gameService -> SendSvrdMsg(p_gameService -> GetAuthSvrdMsgRoute(), (MSGID_GMAE_OVER | ID_REQ), 0, 0, req.SerializeAsString(),0);
    
    //移除table和player
    printf("游戏结束\n");
    table -> m_statu = room_statu::GAME_OVER;
}


void CWZQGame::UndoPlayerAsk(int action_id)
{
    printf("开始处理悔棋逻辑\n");
    std::unique_lock<std::mutex> lock(m_mutex);
    
    auto& table = m_pTable;
    auto& p_gameService = table -> m_pService;
    auto& p_playerMgr = p_gameService->m_pPlayerMgr;
    
    if(p_playerMgr->m_playersMap.find(action_id)== p_playerMgr->m_playersMap.end())
    {
        std::cout<<"error!!! m_pPlayerMgr->m_playersMap找不到 action_id:"<<action_id<<std::endl;
        return;
    }
    auto& action_player = p_playerMgr->m_playersMap[action_id];
    

    //判断该用户是否应该悔棋
    if(m_nCurPutChessID == action_id)
    {
        //当前轮到了该用户下棋，悔棋失败
        printf("当前轮到了该用户下棋，悔棋失败,悔棋的用户id:%d\n",action_id);
        ACKAction ack_action;
        ack_action.set_action_id(action_id);
        ack_action.set_result_msg("undo_chess_failed");
        p_gameService -> SendSvrdMsg(p_gameService -> GetConnSvrdMsgRoute(), (MSGID_ACTION | ID_ACK), 0, 0, ack_action.SerializeAsString(), action_player->m_nClientID);
        return;
    }
    
    
    int another_player_id = m_nWhitePlayerID == action_id ? m_nBlackPlayerID : m_nWhitePlayerID;
    
    if( p_playerMgr->m_playersMap.find(another_player_id)== p_playerMgr->m_playersMap.end())
    {
        std::cout<<"error!!! m_pPlayerMgr->m_playersMap找不到 another_player_id:"<<another_player_id<<std::endl;
        return;
    }
    auto& another_player = p_playerMgr->m_playersMap[another_player_id];
    
    if(m_gameTimer!=nullptr)
    {
        //删除自动下棋的计时器
        CMcgTimer::DeleteTimer(m_gameTimer,true);
        m_gameTimer=nullptr;
    }
    //启动一个自动拒绝的定时器，防止对手不同意悔棋
    m_undoTimer = CMcgTimer::CreateTimer(CWZQGame::AutoRefuseUndoChess, this, OUTTIME_TIME/2, OUTTIME_TIME/2);
    
    
    //需要构建消息询问another_player是否同意action_player悔棋
    m_bIsUndoChcess = true; //该房间正在悔棋
    
    REQAskUndoChess req_ask_undo_chess;
    req_ask_undo_chess.set_table_id(table -> m_nTableID);
    req_ask_undo_chess.set_action_id(action_id);
    printf("询问对手是否同意悔棋\n");
    p_gameService -> SendSvrdMsg(p_gameService -> GetConnSvrdMsgRoute(), (MSGID_ASK_UNDO_CHESS | ID_REQ), 0, 0, req_ask_undo_chess.SerializeAsString(), another_player->m_nClientID);
    
    //给悔棋的玩家发送一个悔棋等待的消息
    ACKAction ack_action;
    ack_action.set_action_id(action_id);
    ack_action.set_result_msg("undo_chess_wait");
    p_gameService -> SendSvrdMsg(p_gameService -> GetConnSvrdMsgRoute(), (MSGID_ACTION | ID_ACK), 0, 0, ack_action.SerializeAsString(), action_player->m_nClientID);
}


void CWZQGame::UndoChess(int action_id,bool is_agree)
{
    auto& table = m_pTable;
    auto& p_gameService = table -> m_pService;
    auto& p_playerMgr = p_gameService->m_pPlayerMgr;
    
    if( p_playerMgr->m_playersMap.find(action_id)== p_playerMgr->m_playersMap.end())
    {
        std::cout<<"error!!! m_pPlayerMgr->m_playersMap找不到 action_id:"<<action_id<<std::endl;
        return;
    }
    auto& action_player = p_playerMgr->m_playersMap[action_id];
    
    int another_player_id = m_nWhitePlayerID == action_id ? m_nBlackPlayerID :  m_nWhitePlayerID;
    
    if( p_playerMgr->m_playersMap.find(another_player_id)== p_playerMgr->m_playersMap.end())
    {
        std::cout<<"error!!! m_pPlayerMgr->m_playersMap找不到 another_player_id:"<<another_player_id<<std::endl;
        return;
    }
    auto& another_player = p_playerMgr->m_playersMap[another_player_id];
    
    std::unique_lock<std::mutex> lock(m_mutex);
    
    if(m_undoTimer!=nullptr)
    {
        //删掉悔棋的计时器
        CMcgTimer::DeleteTimer(m_undoTimer,true);
        m_undoTimer = nullptr;
    }

    m_bIsUndoChcess = false; //该房间悔棋完成
    
    if(is_agree == false)
    {
        //不同意悔棋，直接发给action_player 这个消息
        ACKAction ack_action;
        ack_action.set_action_id(action_id);
        ack_action.set_result_msg("undo_chess_failed");
        p_gameService->SendSvrdMsg(p_gameService->GetConnSvrdMsgRoute(), (MSGID_ACTION | ID_ACK), 0, 0, ack_action.SerializeAsString(), action_player->m_nClientID);
        p_gameService->SendSvrdMsg(p_gameService->GetConnSvrdMsgRoute(), (MSGID_ACTION | ID_ACK), 0, 0, ack_action.SerializeAsString(), another_player->m_nClientID);
        
        //重新创建定时器
        m_gameTimer = CMcgTimer::CreateTimer(CWZQGame::AutoPutChess, this, m_remain_time*1000, OUTTIME_TIME);
    }
    else if(is_agree == true)
    {
        //同意悔棋，需要先拿到悔棋的行和列
        ACKAction ack_action;
        ack_action.set_action_id(action_id);
        ack_action.set_result_msg("undo_chess_sucess");
        //判断action_id是黑棋还是白棋
        std::pair<int,int> position_pair;
        if(action_id == m_nBlackPlayerID)
        {
            printf("悔棋的玩家是黑棋，id: %d\n",action_id);
            //是黑棋玩家，获取黑棋队列的行和列
            position_pair = m_blackPlayerActions.back();
            m_blackPlayerActions.pop_back();
            m_board[position_pair.first][position_pair.second] = 0;
        }
        else if(action_id == m_nWhitePlayerID)
        {
            printf("悔棋的玩家是白棋，id: %d\n",action_id);
            //是白棋玩家，获取白棋队列的行和列
            position_pair = m_whitePlayerActions.back();
            m_whitePlayerActions.pop_back();
            m_board[position_pair.first][position_pair.second] = 0;
        }
        else
        {
            printf("error!!! 无法获取当前这个用户的悔棋队列,%d\n",action_id);
            return;
        }
        printf("悔棋的行 %d,列 %d\n",position_pair.first,position_pair.second);
        
        ack_action.set_action_board_row(position_pair.first);
        ack_action.set_action_board_col(position_pair.second);
        
        
        //修改当前玩家
        m_nCurPutChessID = (m_nCurPutChessID == m_nWhitePlayerID?  m_nBlackPlayerID : m_nWhitePlayerID);
        
        //发给两个玩家悔棋消息
        p_gameService->SendSvrdMsg(p_gameService->GetConnSvrdMsgRoute(), (MSGID_ACTION | ID_ACK), 0, 0, ack_action.SerializeAsString(), action_player->m_nClientID);
        p_gameService->SendSvrdMsg(p_gameService->GetConnSvrdMsgRoute(), (MSGID_ACTION | ID_ACK), 0, 0, ack_action.SerializeAsString(), another_player->m_nClientID);
        
        //重新创建定时器
        m_gameTimer = CMcgTimer::CreateTimer(CWZQGame::AutoPutChess, this, OUTTIME_TIME, OUTTIME_TIME);
    }
}

void CWZQGame::PlayerForceExitGame(int exit_player_id)
{
    printf("退出的用户id:%d\n",exit_player_id);
    
    auto& table = m_pTable;
    auto& p_gameService = table -> m_pService;
    auto& p_playerMgr = p_gameService->m_pPlayerMgr;
    
    if( p_playerMgr->m_playersMap.find(exit_player_id)== p_playerMgr->m_playersMap.end())
    {
        std::cout<<"error!!! m_pPlayerMgr->m_playersMap找不到这个强制退出的用户id"<<std::endl;
        return;
    }
    auto& player1 = p_playerMgr->m_playersMap[exit_player_id];
    
    int player2_id = m_nWhitePlayerID == exit_player_id ? m_nBlackPlayerID :  m_nWhitePlayerID;
    if( p_playerMgr->m_playersMap.find(player2_id)== p_playerMgr->m_playersMap.end())
    {
        std::cout<<"error!!! m_pPlayerMgr->m_playersMap找不到另一个用户:"<<player2_id<<std::endl;
        return;
    }
    auto& player2 = p_playerMgr->m_playersMap[player2_id];
    
    std::cout<<"player1 id:"<<player1->m_nUserID<<"  player2 id:"<<player2->m_nUserID<<std::endl;
    
    int client_id1 = player1->m_nClientID;
    int client_id2 = player2->m_nClientID;
    
    ACKExitGame ack;
    ack.set_code(0);
    ack.set_result_msg("enemy_exit_game");
    ack.set_exit_table_id(table -> m_nTableID);
    ack.set_exit_user_id(exit_player_id);
    
    if(exit_player_id ==player1->m_nUserID)
    {
        std::cout<<"debug1.... 发给玩家对手强制退出的消息，玩家id:"<<player2->m_nUserID<<"  连接id:"<<client_id2<<std::endl;
        p_gameService->SendSvrdMsg( p_gameService->GetConnSvrdMsgRoute(), (MSGID_EXIT_GAME | ID_ACK), 0, 0, ack.SerializeAsString(), client_id2);
        GameOver(player2->m_nUserID,exit_player_id,table -> m_nTableID,100,200,true);
    }
    else if(exit_player_id ==player2->m_nUserID)
    {
        std::cout<<"debug2.... 发给玩家对手强制退出的消息，玩家id:"<<player1->m_nUserID<<"  连接id:"<<client_id1<<std::endl;
        p_gameService->SendSvrdMsg(p_gameService->GetConnSvrdMsgRoute(), (MSGID_EXIT_GAME | ID_ACK), 0, 0, ack.SerializeAsString(), client_id1);
        GameOver(player1->m_nUserID,exit_player_id,table -> m_nTableID,100,200,true);
    }
    else
    {
        std::cout<<"error,找不到这个退出的玩家"<<std::endl;
    }
    p_gameService->m_pPlayerMgr->RemovePlayer(exit_player_id);
    
}

void CWZQGame::Clear()
{
    m_nCurPutChessID = m_nBlackPlayerID;
    m_bIsUndoChcess = false;
    m_blackPlayerActions.clear();
    m_whitePlayerActions.clear();
    for (int i = 0; i < m_board.size(); ++i)
    {
        for (int j = 0; j < m_board[i].size(); ++j)
        {
            m_board[i][j] = 0;
        }
    }
    m_nBlackPlayerOutTimeCount=0;
    m_nWhitePlayerOutTimeCount=0;
    
}

void* CWZQGame::AutoPutChess(void* ptr,unsigned int a)
{
    CWZQGame* p_game = static_cast<CWZQGame*>(ptr);
    
    
    if(p_game->m_pTable->m_statu == GAME_START)
    {
        printf("触发自动下棋，黑白棋的id:%d,%d\n",p_game->m_nBlackPlayerID,p_game->m_nWhitePlayerID);
        //使用机器人自动走子
        int action_id = p_game -> m_nCurPutChessID;
        int row = 0;
        int col = 0;
        
        AutoRobot robot(p_game->m_board.size());
        robot.m_color_id = (action_id ==p_game->m_nBlackPlayerID ?BLACK_CHESS: WHITE_CHESS);
        std::pair<int,int> pos = robot.GetkeySetchess(p_game->m_board);
        row = pos.second;
        col = pos.first;
        
        if(action_id == p_game->m_nWhitePlayerID)
        {
            p_game->m_nWhitePlayerOutTimeCount++;
        }
        else
        {
            p_game->m_nBlackPlayerOutTimeCount++;
        }
        
        p_game -> PutChess(action_id, row, col);
    }
    
    return nullptr;
}

void* CWZQGame::AutoRefuseUndoChess(void* ptr,unsigned int a)
{
    CWZQGame* p_game = static_cast<CWZQGame*>(ptr);
    if(p_game->m_bIsUndoChcess == true)
    {
        printf("触发自动拒绝悔棋，黑白棋的id:%d,%d\n",p_game->m_nBlackPlayerID,p_game->m_nWhitePlayerID);
        //正在悔棋，发送拒绝悔棋
        int action_id = (p_game->m_nWhitePlayerID ==p_game->m_nCurPutChessID ? p_game->m_nBlackPlayerID: p_game->m_nWhitePlayerID);
        p_game->UndoChess(action_id,false);
    }
    
    return nullptr;
}
