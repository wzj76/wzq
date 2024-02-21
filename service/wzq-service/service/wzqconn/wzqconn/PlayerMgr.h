#ifndef __PLAYER_MGR_H__
#define __PLAYER_MGR_H__

#include <map>
#include <google/protobuf/stubs/common.h>
using google::protobuf::int32;
using google::protobuf::int64;
using std::map;

class CConnService;
class CPlayer;

class CPlayerMgr
{
public:
	CPlayerMgr();
	virtual ~CPlayerMgr();
public:	
	CPlayer* CreatePlayer(int64 user_id, const char* skey, int* err_code);	//
	void RemovePlayer(int64 user_id);										//
	CPlayer* GetPlayer(int64 user_id);										//
	int Size();
public:
	map<int64, CPlayer*> m_playersMap;
	CConnService* m_pService;
};

#endif
