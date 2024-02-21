#ifndef __REDISINTF_H__
#define __REDISINTF_H__

#include <stdlib.h>
#include <map>
#include <deque>
#include <vector>
#include <string>
#include <set>
#include <algorithm>
#include <hiredis/hiredis.h>
#include <hiredis/async.h>
#include <hiredis/adapters/libevent.h>
#include "mcgcommon/datatype.h"
#include "Tools.h"

#define REDIS_TIMEOUT_CONNECT   10   // 连接超时（秒）
#define REDIS_TIMEOUT_RW   10        // 读写超时（秒）

/**
 用法：
    CRedisIntf* pri = new CRedisIntf("cache");
    pri->init("127.0.0.1", 6379, "123", 10, 10);
 
    CRedisIntf* pri = new CRedisIntf();
    pri->setTag("msgque");
    pri->init("127.0.0.1", 6379, "123", 10, 10);
 
 注意：
    连接超时、读写超时分别设置，单位秒，业务根据需要各自设置。
    连接超时时间默认10秒；
    读写超时时间默认10秒；
 */
class CRedisIntf
{
public:
	CRedisIntf();
    CRedisIntf(const std::string& tag);
	~CRedisIntf();
            
    /// 初始化 Redis 连接对象，支持设置连接超时和读写超时；
    /// @param ip 地址
    /// @param port 端口
    /// @param pwd 密码
    /// @param db 指定数据库，默认 0;
    /// @param tt_connect 连接超时时间，默认10秒；
    /// @param tt_rw  读写超时时间，默认10秒；
    bool init(const char* ip, int port, const char* pwd, int tt_connect, int tt_rw, int db = 0);
    bool init(const char* ip, int port, const char* pwd, int db = 0);
    inline void setTag(const std::string& tag) { m_strTag = tag; };

    //  redis命令封装：
    //  list 命令：
    int list_llen(const std::string& key);
	int list_rpush(const std::string& key, const std::string& value);
    int list_rpop(const std::string& key , std::string& value);
    int list_lpush(const std::string& key, const std::string& value);
    int list_lpop(const std::string& key, std::string& value);
    int list_blpop(const std::string& key, std::string& value, int timeout);
    int list_blpop(const std::string& key1, const std::string& key2, std::string& value, int timeout);
    int list_blpopv2(const std::vector<std::string>& vKeys, std::string& key, std::string& value, int timeout);
    int list_rem(const std::string& key, const std::string& value, int type = 0);
    int list_range(const std::string& key, std::vector<int64>& values, int begin =0, int end=-1);
    int list_range(const std::string& key, std::vector<std::string>& values, int begin =0, int end=-1);
    int list_rangeV2(const std::string& key, std::vector<int>& values, int begin, int end);
    int list_index(const std::string& key, long long index, std::string& value);

    //  set 命令：
    //  设置过期时间
    int set_expire(const std::string& key, int time);
    //  解除过期时间
    int set_persist(const std::string& key);
    int set_scard(const std::string& key);
	int set_add(const std::string& key, const std::string& value);
    int set_members(const std::string& key, std::vector<int64>& values);
    int set_members(const std::string& key, std::vector<std::string>& values);
    int set_rem(const std::string& key, const std::string& value);
    int set_ismember(const std::string& key, const std::string& val);
    int set_ismember(const std::string& key, std::string value, int& result);

    //  zset(sorted set) 命令：
	int zset_incrby(const std::string& key, long long increment,const std::string& member);
	int zset_add(const std::string& key, long long score,const std::string& member);
    int zset_rank(const std::string& key, const std::string& member, int& rank);
    int zset_revrank(const std::string& key, const std::string& member, int& rank);
    int zset_zscore(const std::string& key, const std::string& member,std::string& value);

    //  hash 命令
	int hash_get(const std::string& key, const std::string& field, std::string& value);
	int hash_set(const std::string& key, const std::string& field, const std::string& value);
    int hash_mset(const std::string& key, const std::map<std::string,std::string>& values);
	int hash_mget(const std::string& key, const std::vector<std::string>& fields, std::map<std::string,std::string>& values);
	int hash_mset(const std::string& key, const std::vector<std::string>& fields, const std::vector<std::string>& values);
    int hash_mset_v2(const std::string& key, const std::vector<std::string>& fields, const std::vector<std::string>& values);
	int hash_getall(const std::string& key, std::map<std::string,std::string>& values);
	int hash_del(const std::string& key,const std::vector<std::string>& fields);
    int hash_del(const std::string& key,const std::string& field);
	int hash_incrby(const std::string& key, const std::string& field,long long increment);
    int hash_incrby(const std::string& key, const std::string& field,long long increment, int64& left);
    int hash_incrby(const std::string& key, const std::string& field,long long increment, int& left);
    int hash_incrby_v2(const std::string& key, const std::string& field,long long increment, int& left);
    int hash_setnx(const std::string& key, const std::string& field, const std::string& value);
	
    //  lua script 
    int load_lua_script(const std::string& name, const std::string& content);
	int evalsha(const std::string& sha1, const std::vector<std::string>& keys, const std::vector<std::string>& argvs);
    int eval(const std::string& content, const std::vector<std::string>& keys, const std::vector<std::string>& argvs,
             std::vector<int64>& vRetVal);
    int evalscript(const std::string& name, const std::vector<std::string>& keys, const std::vector<std::string>& argvs);
    int evalscript(const std::string& name, const std::vector<std::string>& keys, const std::vector<std::string>& argvs, std::vector<int64>& vRetVal);
    int evalscript(const std::string& name, const std::string& content, const std::vector<std::string>& keys, const std::vector<std::string>& argvs, std::vector<int64>& vRetVal);
    
    //  key
    bool key_exists(const std::string& key);
    bool key_hexists(const std::string& key, const std::string& field);
	int del(const std::string& key);
    int keys(const std::string& key, std::vector<std::string>& values);
    
    //  string
    int string_set(const std::string& key, const std::string& value);
	int string_setv2(const std::string& key, const std::string& value , int32 ex);
    int string_setnx(const std::string& key, const std::string& value);
	int string_get(const std::string& key, std::string& value);
    int string_incrby(const std::string& key, long long increment, int64& left);
    //  其它    
    int publish(const std::string& channel, const std::string& msg);
    int scan(const std::string& key, std::vector<std::string>& values, int iter);
	int lock(const std::string& name, const std::vector<std::string>& keys, const std::vector<std::string>& argvs , std::vector<int64>& vRetVal, int64 block_timeout);
private:
    // 连接命令组
    bool _connect(const char* ip, int port, int tt_connect, int tt_rw);
    bool _select_db(int nDB = 0);
	bool _auth(const char* pwd);
	void _disconnect();
	bool _check_connect();
    redisReply* redisCommandArgvUtil(const std::string& cmd , const std::vector<std::string>& vals);
    //  初始化：连接、鉴权、数据库设置；
    bool _initialize();
private:
	redisContext* m_pRedis;
	int m_nDB;
	std::string m_strIp;
	int m_nPort;
	std::string m_strPasswd;
    /// 超时参数
    int m_ntt_connect;      //  连接超时；
    int m_ntt_rw;           //  读写超时
	std::map<std::string,std::string> m_scriptLua;
    
    //  标记，方便追踪；
    std::string m_strTag;
};

#endif
