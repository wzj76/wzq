#include "PlayerMgr.h"
#include "GameService.h"
#include "Player.h"
#include "mcgevent.h"
#include "log.h"
#include "AppDef.h"
#include <stdio.h>

extern CService *g_pService;

CPlayerMgr::CPlayerMgr()
{
	m_pService = (CGameService *)g_pService;
}

CPlayerMgr::~CPlayerMgr()
{
}

int CPlayerMgr::Size()
{
	return int(m_playersMap.size());
}


void CPlayerMgr::RemovePlayer(int64 user_id)
{
	map<int64,std::shared_ptr<CPlayer>>::iterator iter = m_playersMap.find(user_id);
	if (iter != m_playersMap.end())
	{
		m_playersMap.erase(iter);
	}
}

std::shared_ptr<CPlayer> CPlayerMgr::GetPlayer(int64 user_id)
{
    std::shared_ptr<CPlayer> player = NULL;
	map<int64,std::shared_ptr<CPlayer>>::iterator iter = m_playersMap.find(user_id);
	if (iter != m_playersMap.end())
	{
		player = iter ->second;
	}

	return player;
}

std::shared_ptr<CPlayer> CPlayerMgr::CreatePlayer(int64 user_id,int client_id)
{
    std::shared_ptr<CPlayer> pPlayer = NULL;
    auto iter = m_playersMap.find(user_id);
    if (iter != m_playersMap.end())
    {
        pPlayer = iter ->second;
    }
    else
    {
        pPlayer = std::make_shared<CPlayer>(user_id,client_id);
        if (!pPlayer->Init())
        {
            return NULL;
        }
        m_playersMap.insert(std::make_pair(user_id, pPlayer));
    }
    return pPlayer;
}
