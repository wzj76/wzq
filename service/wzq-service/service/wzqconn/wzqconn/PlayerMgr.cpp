#include "PlayerMgr.h"
#include "ConnService.h"
#include "ConnClient.h"
#include "Player.h"
#include "mcgevent.h"
#include "log.h"
#include "AppDef.h"
#include <stdio.h>

extern CService *g_pService;

CPlayerMgr::CPlayerMgr()
{
	m_pService = (CConnService*)g_pService;
}

CPlayerMgr::~CPlayerMgr()
{
}

int CPlayerMgr::Size()
{
	return (int)m_playersMap.size();
}

CPlayer* CPlayerMgr::CreatePlayer(int64 user_id, const char* skey, int* err_code)
{
	CPlayer* pPlayer = NULL;
	map<int64,CPlayer*>::iterator iter = m_playersMap.find(user_id);
	if (iter != m_playersMap.end())
	{
		pPlayer = m_playersMap[user_id];
	}
	else
	{	
		//
		pPlayer = new CPlayer(user_id);
		if (!pPlayer->Init())
		{
			delete pPlayer;
			return NULL;
		}
		m_playersMap[user_id] = pPlayer;
	}

	return pPlayer;
}

void CPlayerMgr::RemovePlayer(int64 user_id)
{
	map<int64,CPlayer*>::iterator iter = m_playersMap.find(user_id);
	if (iter != m_playersMap.end())
	{
        if(iter ->second) delete iter ->second;
        
		m_playersMap.erase(iter);
	}
}

CPlayer* CPlayerMgr::GetPlayer(int64 user_id)
{
	CPlayer* player = NULL;
	map<int64,CPlayer*>::iterator iter = m_playersMap.find(user_id);
	if (iter != m_playersMap.end())
	{
		player = m_playersMap[user_id];
	}

	return player;
}
