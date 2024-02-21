#include "Player.h"
#include "Table.h"
#include "mcgevent.h"
#include "log.h"
#include "TableMgr.h"
#include "PlayerMgr.h"
#include "ProtocolDef.h"

#include "SeqUtil.h"

extern CService *g_pService;

class CGameService;

CPlayer::CPlayer(int user_id,int client_id)
{
	m_nUserID = user_id;
    m_nClientID = client_id;
	m_pService = (CGameService *)g_pService;
}

CPlayer::~CPlayer()
{
}

bool CPlayer::Init()
{
	return true;
}

void CPlayer::Disconnect()
{
	mcgWriteLog("Info Player(%p)(ID = %lld, tableID=%d)::Disconnect, disconnect begin", this, m_nUserID, m_nTableID);
    mcgWriteLog("Info Player(ID = %lld)::Disconnect, disconnect end", m_nUserID);
}

int CPlayer::GetTableID()
{
	return m_nTableID;
}


bool CPlayer::SendMsg(DWORD dwMsgID, PROTOBUF_MESSAGE* pMsg, DWORD dwSeqID)
{
    return true;
}


bool CPlayer::OnMsg(DWORD dwMsgID, DWORD dwSeqID, const byte* pBuffer, int nLen)
{
	bool bResult = true;
	bool bHandled = true;
	switch(dwMsgID)
	{
	default:
		bHandled = false;
		break;
	}
	if (!bHandled)
	{
        std::shared_ptr<CTable> pTable = m_pService->GetTableByID(m_nTableID);
		if (IsNotNULL(pTable))
		{
			int nRet = pTable->OnClientMessage(m_nUserID, dwMsgID,dwSeqID,pBuffer,nLen);
			if (nRet != 0)
			{
				bResult = false;
			}
		}
	}
	
	return bResult;
}














