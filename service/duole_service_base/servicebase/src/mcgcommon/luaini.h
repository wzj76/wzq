#ifndef __LUA_INI_H__
#define __LUA_INI_H__

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
};

#include <string>
using namespace std;

#ifdef WIN32
#pragma comment(lib,"LuaLib.lib") 
#else

#endif

#define TABLE_ITEM_SEP	 ";"
 
class CLuaIni
{
public:
	CLuaIni();
	~CLuaIni();

public:
	bool LoadFile(const char *szLuaFilePath);		// 载入lua文件
	int GetInt(const char *szFieldName);			// 读取整形
	char* GetString(const char *szFieldName);		// 读取字符串
	bool GetBool(const char *szFieldName);		// 读取布尔型
	double GetDouble(const char *szFieldName);	// 读取浮点型
	// Table处理
	char* GetTableItemByName(const char *szTableName, const char *szItemName);		// 读取表子节点字符串
	char* GetTableItemByIndex(const char* szTableName, int index);			// 读取表子节点字符串
	void GetTableAllItem(const char *szTableName, string &strAllItemBySep);		// 读取表所有子节点字符串
	
private:
	lua_State *m_pLuaState;					// lua指针
};

#endif	// __LUA_INI_H__

