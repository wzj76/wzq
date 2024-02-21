#include "Table.h"
#include "WZQGame.h"
#include "mcgevent.h"
#include "globalfunc.h"
#include "log.h"
#include "PlayerMgr.h"
#include "ProtocolDef.h"

#include "Player.h"
#include "WZQChat.hpp"

extern CService *g_pService;
class CGameService;

CTable::CTable(int nTableID)
{
	m_pGame = nullptr;
	m_nTableID = nTableID;
	m_hFixTimer = NULL;
    m_pService = (CGameService *)g_pService;
    
    m_statu=room_statu::GAME_START;
    m_playerCount = 2;
}


CTable::~CTable()
{
    printf("CTable::~CTable()\n");
	if (IsNotNULL(m_hFixTimer))
	{
		CMcgTimer::DeleteTimer(m_hFixTimer, true);
	}

	if (IsNotNULL(m_pGame))
	{
		delete m_pGame;
	}
    
    if (IsNotNULL(m_pChat))
    {
        delete m_pChat;
    }
}

void CTable::CreateGame(int black_id,int white_id)
{
    m_pGame = new CWZQGame(nullptr,this,black_id,white_id);
    m_pChat = new CWZQChat(this);
}



bool CTable::Init()
{
    mcgWriteLog("CTable::Init  ");
	m_pService=(CGameService *)g_pService;
	m_pGame = NULL;
	m_hFixTimer = CMcgTimer::CreateTimer((LPTHREAD_START_ROUTINE)PTimerOut, this, 1000, 1000);
	return true;
}

DWORD CTable::OnPTimerOut()
{
	if (IsNotNULL(m_pGame))
	{
		m_pGame->OnTimer();
	}

	return 0;
}

int CTable::OnClientMessage(int64 user_id, DWORD dwMsgID, DWORD dwSeqID, const byte* pBuffer, int nLen)
{
	return m_pGame->OnClientMessage(user_id,dwMsgID,dwSeqID,pBuffer,nLen);
}

int CTable::GetTableID()
{
	return m_nTableID;
}

bool CTable::SendClientMsg(DWORD dwMsgID, DWORD dwSeqID, PROTOBUF_MESSAGE* pMsg)
{
    return false;
}

bool CTable::SendClientMsgTo(int64 user_id, DWORD dwMsgID, DWORD dwSeqID, PROTOBUF_MESSAGE* pMsg)
{
    std::shared_ptr<CPlayer> pPlayer = m_pService->GetPlayerByID(user_id);
	if (IsNULL(pPlayer))
	{
		mcgWriteLog("Error Table(ID = %d)::SendClientMsgTo,not exists the player %lld",m_nTableID,user_id);
		return false;
	}
	return pPlayer->SendMsg(dwMsgID,pMsg,dwSeqID);
}


void CTable::Clear()
{
    m_pGame->Clear();
    m_continueIds.clear();
    m_statu=GAME_START;
    
}

void CTable::ContinueGame(int user_id,int enemy_id)
{
    
    auto& pPlayerMgr = m_pService -> m_pPlayerMgr;
    if (pPlayerMgr->m_playersMap.find(user_id) == pPlayerMgr->m_playersMap.end())
    {
        mcgWriteLog("找不到user_id:%d",user_id);
        return;
    }
    auto player1 = pPlayerMgr ->m_playersMap[user_id];
    
    printf("table ->m_playerCount:%d\n",m_playerCount);
    if(pPlayerMgr->m_playersMap.find(enemy_id) == pPlayerMgr->m_playersMap.end())
    {
        //说明对手退出游戏了
        ACKContinueGame ack;
        ack.set_code(1);
        ack.set_result_msg("对手退出游戏房间，无法继续游戏");
        
        m_pService->SendSvrdMsg(m_pService->GetConnSvrdMsgRoute(), (MSGID_CONTINUE_GAME | ID_ACK), 0, 0, ack.SerializeAsString(), player1->m_nClientID);
        return;
    }
    auto player2 = pPlayerMgr ->m_playersMap[enemy_id];
    

    printf("询问对手是否同意继续游戏\n");
    if(m_continueIds.size()==0)
    {
        m_continueIds.push_back(user_id);
    }
    else if(m_continueIds.size() ==1&&m_continueIds[0]==enemy_id)
    {
        //说明两个人同时发起了继续游戏的请求，直接给他们创建房间
        //创建table成功，给两个player发送table创建成功的ack
        ACKCreatGame ack;
        ack.set_result_msg("create_game_sucess");
        ack.set_code(0);
        ack.set_room_id(m_nTableID);
        ack.set_black_id(m_pGame->m_nBlackPlayerID);
        ack.set_white_id(m_pGame->m_nWhitePlayerID);
        ack.set_score(100);
        ack.set_coins(200);
        Clear();
        
        m_pService->SendSvrdMsg(m_pService->GetConnSvrdMsgRoute(), (MSGID_CREATE_GAME | ID_ACK), 0, 0, ack.SerializeAsString(), player1->m_nClientID);
        m_pService->SendSvrdMsg(m_pService->GetConnSvrdMsgRoute(), (MSGID_CREATE_GAME | ID_ACK), 0, 0, ack.SerializeAsString(), player2->m_nClientID);
        return;
    }
    ACKContinueGame ack;
    ack.set_code(0);
    ack.set_result_msg("询问对手是否同意继续游戏");
    m_pService->SendSvrdMsg(m_pService->GetConnSvrdMsgRoute(), (MSGID_CONTINUE_GAME | ID_ACK), 0, 0, ack.SerializeAsString(), player1->m_nClientID);
    
    
    REQAskContinueGame req1;
    req1.set_enemy_user_id(user_id);
    req1.set_user_id(enemy_id);
    req1.set_table_id(m_nTableID);
    m_pService->SendSvrdMsg(m_pService->GetConnSvrdMsgRoute(), (MSGID_ASK_CONTINUE_GAME | ID_REQ), 0, 0, req1.SerializeAsString(), player2->m_nClientID);
}
void CTable::AckContinueGame(bool is_agree,int user_id,int enemy_id)
{
    auto& pPlayerMgr = m_pService -> m_pPlayerMgr;
    if (pPlayerMgr->m_playersMap.find(user_id) == pPlayerMgr->m_playersMap.end())
    {
        mcgWriteLog("找不到user_id:%d",user_id);
        return;
    }
    auto player1 = pPlayerMgr ->m_playersMap[user_id];
    if (pPlayerMgr->m_playersMap.find(enemy_id) == pPlayerMgr->m_playersMap.end())
    {
        mcgWriteLog("找不到enemy_id:%d",enemy_id);
        return;
    }
    auto player2 = pPlayerMgr ->m_playersMap[enemy_id];
    
    if(is_agree==true)
    {
        //同意继续游戏，TODO
        //创建table成功，给两个player发送table创建成功的ack
        ACKCreatGame ack;
        ack.set_result_msg("create_game_sucess");
        ack.set_code(0);
        ack.set_room_id(m_nTableID);
        ack.set_black_id(m_pGame->m_nBlackPlayerID);
        ack.set_white_id(m_pGame->m_nWhitePlayerID);
        ack.set_score(100);
        ack.set_coins(200);
        Clear();
        
        m_pService->SendSvrdMsg(m_pService->GetConnSvrdMsgRoute(), (MSGID_CREATE_GAME | ID_ACK), 0, 0, ack.SerializeAsString(), player1->m_nClientID);
        m_pService->SendSvrdMsg(m_pService->GetConnSvrdMsgRoute(), (MSGID_CREATE_GAME | ID_ACK), 0, 0, ack.SerializeAsString(), player2->m_nClientID);
    }
    else
    {
        ACKRefuseContinueGame ack1;
        ack1.set_code(1);
        ack1.set_result_msg("对方拒绝继续游戏");
        m_continueIds.clear();

        m_pService->SendSvrdMsg(m_pService->GetConnSvrdMsgRoute(), (MSGID_REFUSE_GAME | ID_ACK), 0, 0, ack1.SerializeAsString(), player2->m_nClientID);
    }
}
