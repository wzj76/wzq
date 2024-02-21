#include "Player.h"
#include "mcgevent.h"
#include "log.h"
#include "PlayerMgr.h"
#include "ProtocolDef.h"
#include "ConnClient.h"
#include "SeqUtil.h"
#include "./proto/wzqProtocol.pb.h"

extern CService *g_pService;

class CGameService;

CPlayer::CPlayer(int64 user_id)
{
	m_pClient = NULL;
	m_nUserID = user_id;
	m_pService = (CConnService *)g_pService;
}

CPlayer::~CPlayer()
{
}

bool CPlayer::Init()
{
	return true;
}

void CPlayer::Disconnect(bool needNtf)
{
    //玩家断线
    printf("玩家断线,id:%d\n",m_nUserID);
    
    //TODO 应该给room服务和game服务发送异常退出的消息，因为这个用户可能正在匹配，也有可能在游戏
    REQForcExit req;
    req.set_user_id(m_nUserID);
    MsgPackage msg;
    msg.set_msg(req.SerializeAsString());
    
    //发给room一个消息
    printf("发给room一个强制退出的命令\n");
    m_pService->SendSvrdMsg(m_pService->GetRoomSvrdMsgRoute(), MSGID_FORCE_EXIT | ID_REQ, 0, m_nUserID, msg.msg(), 0);
    printf("发给game一个强制退出的命令\n");
    m_pService->SendSvrdMsg(m_pService->GetGameSvrdMsgRoute(), MSGID_FORCE_EXIT | ID_REQ, 0, m_nUserID, msg.msg(), 0);
    return ;
}

bool CPlayer::SendMsg(DWORD dwMsgID, const std::string& strBuffer, DWORD dwSeqID)
{
    if (IsNULL(m_pClient))
	{
		mcgWriteLog("Error,Player(ID = %lld)::SendMsg, m_pClient is NULL", m_nUserID);
		return false;
	}
    
	return m_pClient->SendMsg(dwMsgID, strBuffer, dwSeqID);
}

bool CPlayer::SendMsg(DWORD dwMsgID, PROTOBUF_MESSAGE* pMsg, DWORD dwSeqID)
{
	if (IsNULL(m_pClient))
	{
		mcgWriteLog("Error,Player(ID = %lld)::SendMsg, m_pClient is NULL", m_nUserID);
		return false;
	}

	return m_pClient->SendMsg(dwMsgID, pMsg, dwSeqID);
}

bool CPlayer::OnMsg(DWORD dwMsgID, DWORD dwSeqID, const byte* pBuffer, int nLen)
{
	if (IsNULL(m_pClient))
	{
		return false;
	}
    dwSeqID = 0 ;
	bool bResult = true;
	switch(dwMsgID)
	{
    case MSGID_HOLD | ID_REQ:
        bResult = OnHold(dwSeqID,pBuffer,nLen);
        break;
    }
	return bResult;
}

bool CPlayer::OnHold(DWORD dwSeqID, const byte* pBuffer, int nLen)
{
	ACKHold ack;
    printf("收到心跳包ack\n");
	ack.set_result_id(0);
	SendMsg(MSGID_HOLD | ID_ACK,&ack,dwSeqID);
	return true;
}







