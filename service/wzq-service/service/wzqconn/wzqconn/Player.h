#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "ConnService.h"
#include <list>
class CConnClient ;

class CPlayer
{
public:
	CPlayer(int64 user_id);
	virtual ~CPlayer();
public:
	bool Init();
	void Disconnect(bool needNtf = true);
public:
	bool SendMsg(DWORD dwMsgID, PROTOBUF_MESSAGE* pMsg, DWORD dwSeqID);
    bool SendMsg(DWORD dwMsgID, const std::string& strBuffer, DWORD dwSeqID);
    
	bool OnMsg(DWORD dwMsgID, DWORD dwSeqID, const byte* pBuffer, int nLen);
private:
    //保持连接的消息
    bool OnHold(DWORD dwSeqID, const byte* pBuffer, int nLen);
public:
	CConnClient*                            m_pClient;
	CConnService*                           m_pService;
	int64                                   m_nUserID;
    int                                     m_mClientID;
};

#endif
