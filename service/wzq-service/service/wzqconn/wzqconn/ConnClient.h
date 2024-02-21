#ifndef _GAMEINFO_CLIENT_H_
#define _GAMEINFO_CLIENT_H_

#include "mcgclient.h"
#include "AppDef.h"
#include "Player.h"

using google::protobuf::int32;
using google::protobuf::int64;

class CConnService;

class CConnClient : public CMCGClient
{
public:
	CConnClient(DWORD dwRemoteIP,WORD wListenPort);
	virtual ~CConnClient();

public:
	virtual bool OnInit();
	virtual bool OnMsg(DWORD dwMsgID, DWORD dwSeqID);
	virtual bool OnBreak(DWORD dwType);
	
    virtual bool OnTimeOut(DWORD dwEventID);
public :
    //设置 player
    void SetPlayer(CPlayer* pPlayer){ m_pPlayer = pPlayer ;}
    void SetLoginStatus(bool status){ m_bLogined=status; }
    void RemovePlayer(){m_pPlayer = nullptr; }
private:
	CConnService*     m_pService;
    CPlayer*          m_pPlayer ;
    bool              m_bLogined ;
};

#endif

