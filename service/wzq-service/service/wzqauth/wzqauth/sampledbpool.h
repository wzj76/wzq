#ifndef __SAMPLE_DB_POOL_H__
#define __SAMPLE_DB_POOL_H__

#include <mysql/mysql.h>
#include <string>
#include <vector>
#include <google/protobuf/stubs/common.h>
#include "AppDef.h"
#include "wzqProtocol.pb.h"
using namespace std;

#define IN
#define OUT
#define INOUT
#define MAX_SQL_LEN	4*1024
#define SQL_SUCCESS 0
#define SQL_FAILED  -1

class CSampleDBPool
{
public:
	CSampleDBPool();
	~CSampleDBPool();

public:
	//
    int Connect(const string &strHost, 
            const string &strUser, 
            const string &strPwd, 
            const string &strDBName, 
            unsigned int nPort, 
            const string &strCharSet="utf8");

    //
    int Close();

    //
    int Ping();

	//
    int Commit();

    //
    int Rollback();
    
    //设备信息存储
    int UpdateDeviceInfo(const std::string& deviceid , const std::string& info);
    //查询用户账号信息
    int QueryUserInfo(const std::string& account , int64& userid , std::string& passwd);
    //插入用户账号信息
    int InsertUserInfo(const std::string& account , const std::string& passwd);
    //修改用户头像信息
    int EditUserInfo(const int&user_id, int32 &headnum);
    //修改用户名字
    int EditUserInfo(const int&user_id, std::string &username);
    
    //查找用户对应的道具数量
    int QueryPropNum(const int64&userid,std::vector<std::pair<int,int>>&v_prop);
    
    int QueryUserAllInfo(const int64& userid ,std::string&name,int32&wintimes,int32&losetimes,int32&score,int& level,int32&xp,int32&head_id);
    int QueryUserAllInfo(const int64& userid ,std::string&name,int32&wintimes,int32&losetimes,int32&score,int& level,int32&xp,int32&head_id,int32&coins);
    int QueryUserAllInfo(const std::string& account ,int32&id,int32&wintimes,int32&losetimes,int32&score,int& level,int32&xp,int32&head_id,std::string&username);
    
    
    int QueryRandingList(const int limit,const int offset, vector<int>& ids,vector<string>& accounts,vector<int>&wintimes,vector<int>&losetimes,vector<int>&scores,vector<int>&  levels,vector<int>&xps,vector<int>& head_ids);
    int UpdateWinnerAndLoser(const int&winner_id, const int&loser_id,const int score,const int conis);
    int UpdateGameInfo(const REQGameOver&req);
    int GetGameHistory(int player_id,int limit,int offset,std::vector<GameInfo>&game_infos);
    //查询两个人是否是好友关系
    int QueryIsFriend(int friend_id,int user_id, bool&is_friend);
    //添加好友
    int AddWaitFriend(int friend_id,int user_id);
    //查找friend_id目前有几个好友请求
    int FindAddWaitFriendNum(int friend_id,int&friend_num);
    //从wait_friend移除好友请求
    int RemoveWaitFriend(int user_id,int friend_id);
    //将user_id和friend_id添加到friends表
    int AddFriend(int user_id,int friend_id);
    //获取所有好友的id
    int GetFriendIDs(int user_id,std::vector<int>&friend_ids);
    //获取所有的wait好友id以及time
    int GetWaitFriendIDs(int user_id,std::vector<int>&wait_friend_ids,std::vector<std::string>&times);
    //删除好友
    int DeleteFriend(int user_id,int friend_id);
    //给用户减少金币
    int ReducedCoins(int user_id,int coins_num);
    //判断用户是否购买过这个道具
    int CheckUserHasItem(int user_id,int item_id,bool&is_has);
    //用户没有购买过这个道具，增加这个道具以及数量
    int AddItem(int user_id,int item_id,int num);
    //用户有这个道具，给用户增加道具数量
    int IncreaseItemNum(int user_id,int item_id,int num);
    //减少道具数量
    int ReduceItemNum(int user_id,int item_id,int num);
    //获取该用户拥有的道具数量
    int GetItemNum(int user_id,int item_id,int&num);
    
    //清理超过三十天的历史对局
    int ClearHistoryOutTime(std::string time);
private:
    CSampleDBPool(const CSampleDBPool &dbpool);
    CSampleDBPool & operator=(const CSampleDBPool &dbpool); 
    void StringTOPosition(std::vector<Position>&positions,std::string& s);
    
private:
    MYSQL *m_hMySQL;
	char m_szSQL[MAX_SQL_LEN];
	bool m_bPrint;
};

#endif //__SAMPLE_DB_POOL_H__


