#ifndef __TABLE_MGR_H__
#define __TABLE_MGR_H__

#include <map>
#include <memory>
#include "gameProtocol.pb.h"
using std::map;

class CTable;
class CTableMgr
{
public:
	CTableMgr();
	virtual ~CTableMgr();

    std::shared_ptr<CTable> CreateTable(int nTableID);
    std::shared_ptr<CTable> CreateTable(REQCreatGame&req);
    void PlayerExitGame(REQExitGame&req);
    void PlayerForceExitGame(REQForcExit&req);
    
    std::shared_ptr<CTable> GetTable(int nTableID);
	void RemoveTable(int nTableID);
	int Size();
public:
	map<int, std::shared_ptr<CTable>> m_tablesMap;
    int m_nextTableID;
};

#endif
