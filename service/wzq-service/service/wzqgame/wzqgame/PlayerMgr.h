#ifndef __PLAYER_MGR_H__
#define __PLAYER_MGR_H__

#include <map>
#include <memory>
#include <google/protobuf/stubs/common.h>
using google::protobuf::int32;
using google::protobuf::int64;
using std::map;

class CGameService;
class CPlayer;

class CPlayerMgr
{
public:
	CPlayerMgr();
	virtual ~CPlayerMgr();
public:	
	void RemovePlayer(int64 user_id);										//
    std::shared_ptr<CPlayer> GetPlayer(int64 user_id);										//
	int Size();											//
    std::shared_ptr<CPlayer> CreatePlayer(int64 user_id,int client_id);                                   //获取一个玩家实例
public:
	map<int64, std::shared_ptr<CPlayer>> m_playersMap;
	CGameService* m_pService;
};

#endif
