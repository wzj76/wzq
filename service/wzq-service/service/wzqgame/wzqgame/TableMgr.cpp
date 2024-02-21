#include "TableMgr.h"
#include "AppDef.h"
#include "GameService.h"
#include "Table.h"
#include "log.h"

extern CGameService* g_pService ;

CTableMgr::CTableMgr()
{
    m_nextTableID=1;
}

CTableMgr::~CTableMgr()
{
}

std::shared_ptr<CTable> CTableMgr::CreateTable(int nTableID)
{
    mcgWriteLog("CTableMgr::CreateTable : %d",nTableID);
    std::shared_ptr<CTable> pTable = NULL;
	map<int, std::shared_ptr<CTable>>::iterator iter = m_tablesMap.find(nTableID);
	if (iter != m_tablesMap.end())
	{
        mcgWriteLog("CTableMgr::CreateTable : %d 2",nTableID);
		pTable = iter->second;
	}
	else
	{
        mcgWriteLog("CTableMgr::CreateTable : %d 3",nTableID);
		pTable = std::make_shared<CTable>(nTableID);
		if (!pTable->Init())
		{
            mcgWriteLog("CTableMgr::CreateTable : %d 5",nTableID);
			return NULL;
		}
         mcgWriteLog("CTableMgr::CreateTable : %d 4",nTableID);
		m_tablesMap[nTableID] = pTable;
	}

	return pTable;
}

std::shared_ptr<CTable> CTableMgr::GetTable(int nTableID)
{
    std::shared_ptr<CTable> pTable = NULL;
	map<int, std::shared_ptr<CTable>>::iterator iter = m_tablesMap.find(nTableID);
	if (iter != m_tablesMap.end())
	{
		pTable = iter->second;
	}

	return pTable;
}

void CTableMgr::RemoveTable(int nTableID)
{
	map<int, std::shared_ptr<CTable>>::iterator iter = m_tablesMap.find(nTableID);
	if (iter != m_tablesMap.end())
	{
		m_tablesMap.erase(iter);
	}
}

int CTableMgr::Size()
{
	return int(m_tablesMap.size());
}
