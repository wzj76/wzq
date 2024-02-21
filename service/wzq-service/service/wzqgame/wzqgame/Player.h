#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "GameService.h"

class CGameClient;
class CGameService;

class CPlayer
{
public:
	CPlayer(int user_id,int client_id);
	virtual ~CPlayer();
public:
	bool Init();
	void Disconnect();
	int GetTableID();
public:
	bool SendMsg(DWORD dwMsgID, PROTOBUF_MESSAGE* pMsg, DWORD dwSeqID);
	bool OnMsg(DWORD dwMsgID, DWORD dwSeqID, const byte* pBuffer, int nLen);
public:
	CGameService* m_pService;
	int m_nUserID;
    int m_nClientID;   //该user的链接id
    int m_nTableID;
    
 };

#endif
