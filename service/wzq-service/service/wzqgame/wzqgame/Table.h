#ifndef __TABLE_H__
#define __TABLE_H__

#include "GameService.h"
#include "gameProtocol.pb.h"
#include "mcgtimer.h"
#include "ILog.h"
#include "ProtocolDef.h"
#include <vector>

class CWZQGame;
class CWZQChat;
class CGameService;
typedef enum {GAME_START , GAME_OVER ,GAME_DESTORY} room_statu;


class CTable
{
public:
	CTable(int nTableID);
	~CTable();
public:
	bool Init();
	static LPTHREAD_START_ROUTINE PTimerOut( LPVOID arg ) {((CTable *)arg)->OnPTimerOut();return 0;}
	DWORD OnPTimerOut();
public:
	int OnClientMessage(int64 user_id, DWORD dwMsgID, DWORD dwSeqID, const byte* pBuffer, int nLen);
public:
/*************************************************************************************************************/
	virtual int GetTableID();
	virtual bool SendClientMsg(DWORD dwMsgID, DWORD dwSeqID, PROTOBUF_MESSAGE* pMsg);
	virtual bool SendClientMsgTo(int64 user_id, DWORD dwMsgID, DWORD dwSeqID, PROTOBUF_MESSAGE* pMsg);
    
    
    void CreateGame(int black_id,int white_id);
    void Clear();
    void ContinueGame(int user_id,int enemy_id);
    void AckContinueGame(bool is_agree,int user_id,int enemy_id);
public:
	HANDLE m_hFixTimer;
	CGameService* m_pService;
    CWZQGame* m_pGame;
    CWZQChat* m_pChat;
	int m_nTableID;
    room_statu m_statu; //房间状态
    int m_playerCount;  //房间玩家数量
    //继续游戏的id，如果数量为2，则继续游戏
    std::vector<int> m_continueIds;
};

#endif
