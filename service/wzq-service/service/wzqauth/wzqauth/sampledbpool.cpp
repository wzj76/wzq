#include "sampledbpool.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "log.h"
#include <vector>
#include <chrono>

CSampleDBPool::CSampleDBPool()
: m_hMySQL(NULL), m_bPrint(true)
{
	memset(m_szSQL, 0, MAX_SQL_LEN);

	//mysql_library_int();

	m_hMySQL = mysql_init(0);
	if (NULL == m_hMySQL)
	{
		printf("mysql_init err(%s)\n", mysql_error(m_hMySQL));
	}
}

CSampleDBPool::~CSampleDBPool()
{
	Close();
}

int CSampleDBPool::Connect(const string &strHost, 
						   const string &strUser, 
						   const string &strPwd, 
						   const string &strDBName, 
						   unsigned int nPort, 
						   const string &strCharSet)
{
	//
	if (m_hMySQL != mysql_real_connect(m_hMySQL, strHost.c_str(), strUser.c_str(), strPwd.c_str(), strDBName.c_str(), nPort, 0, 0))
	{
		printf("mysql_real_connect err(%s)\n", mysql_error(m_hMySQL));
		return SQL_FAILED;
	}

	//
	if (mysql_set_character_set(m_hMySQL, strCharSet.c_str()))
	{
		printf("mysql_set_character_set err(%s)\n", mysql_error(m_hMySQL));
		return SQL_FAILED;
	}

	char value = 1 ;
	int ret = mysql_options(m_hMySQL, MYSQL_OPT_RECONNECT, (char *)&value);
	printf("mysql_options ret : %d", ret);

	return SQL_SUCCESS;
}

int CSampleDBPool::Close()
{
	if (m_hMySQL)
	{
		mysql_close(m_hMySQL);
		m_hMySQL = NULL;
	}

	mysql_library_end();

	return SQL_SUCCESS;
}

int CSampleDBPool::Ping()
{
	if (mysql_ping(m_hMySQL))
	{
		printf("mysql_ping err(%s)\n", mysql_error(m_hMySQL));
		return SQL_FAILED;//
	}

	return SQL_SUCCESS;//
}

int CSampleDBPool::Commit()
{
    return mysql_commit(m_hMySQL);
}

int CSampleDBPool::Rollback()
{
    return mysql_rollback(m_hMySQL);
}

//设备信息存储
int CSampleDBPool::UpdateDeviceInfo(const std::string& deviceid , const std::string& info)
{
    mcgWriteLog("CSampleDBPool::UpdateDeviceInfo deviceid : %s , info : %s" , deviceid.c_str() , info.c_str());
    memset(m_szSQL, 0, MAX_SQL_LEN);
    int len = sprintf(m_szSQL, "insert into deviceinfo values('%s', '%s') on duplicate key update info = '%s'", deviceid.c_str(), info.c_str(), info.c_str());
    mcgWriteLog("CSampleDBPool::UpdateDeviceInfo sql : %s" , m_szSQL);
    int ret = mysql_real_query(m_hMySQL, m_szSQL, len);
    if (IsNotZero(ret))
    {
        mcgWriteLog("Error exec query: %s \n", mysql_error(m_hMySQL));
        return SQL_FAILED;
    }
    else
    {
        mcgWriteLog("exec query successfully");
        return SQL_SUCCESS;
    }
    return 0;
}

//查询用户账号信息
int CSampleDBPool::QueryUserInfo(const std::string& account , int64& userid , std::string& passwd)
{
    memset(m_szSQL, 0, MAX_SQL_LEN);
    int len = sprintf(m_szSQL, "select ID , passwd from user where account = '%s'", account.c_str());
    mcgWriteLog("CSampleDBPool::QueryUserInfo sql : %s" , m_szSQL);
    int ret = mysql_real_query(m_hMySQL, m_szSQL, len);
    if (IsNotZero(ret))
    {
        mcgWriteLog("Error exec query: %s \n", mysql_error(m_hMySQL));
        return SQL_FAILED;
    }
    MYSQL_RES *results = mysql_store_result(m_hMySQL);
    MYSQL_ROW record;
    while((record = mysql_fetch_row(results)))
    {
        userid = atol(record[0]);
        passwd = record[1];
        break;
    }
    mysql_free_result(results);
    return 0;
}
//查询用户信息
int CSampleDBPool::QueryUserAllInfo(const int64& userid ,std::string&name,int32&wintimes,int32&losetimes,int32&score,int& level,int32&xp,int32&head_id)
{
    memset(m_szSQL, 0, MAX_SQL_LEN);
    int len = sprintf(m_szSQL, "select username,win_times, lose_times, score, level, XP,head from user where id = '%d'", userid);
    mcgWriteLog("CSampleDBPool::QueryUserInfo sql : %s" , m_szSQL);
    int ret = mysql_real_query(m_hMySQL, m_szSQL, len);
    if (IsNotZero(ret))
    {
        mcgWriteLog("Error exec query: %s \n", mysql_error(m_hMySQL));
        return SQL_FAILED;
    }
    MYSQL_RES *results = mysql_store_result(m_hMySQL);
    MYSQL_ROW record;
    while((record = mysql_fetch_row(results)))
    {
        name = record[0];
        wintimes = atoi(record[1]);
        losetimes = atoi(record[2]);
        score = atoi(record[3]);
        level = atoi(record[4]);
        xp = atoi(record[5]);
        head_id=atoi(record[6]);
        break;
    }
    mysql_free_result(results);
    return 0;
}


//查询用户信息
int CSampleDBPool::QueryUserAllInfo(const int64& userid ,std::string&name,int32&wintimes,int32&losetimes,int32&score,int& level,int32&xp,int32&head_id,int32&coins)
{
    memset(m_szSQL, 0, MAX_SQL_LEN);
    int len = sprintf(m_szSQL, "select username,win_times, lose_times, score, level, XP,head ,coins from user where id = '%d'", userid);
    mcgWriteLog("CSampleDBPool::QueryUserAllInfo sql : %s" , m_szSQL);
    int ret = mysql_real_query(m_hMySQL, m_szSQL, len);
    if (IsNotZero(ret))
    {
        mcgWriteLog("Error exec query: %s \n", mysql_error(m_hMySQL));
        return SQL_FAILED;
    }
    MYSQL_RES *results = mysql_store_result(m_hMySQL);
    MYSQL_ROW record;
    while((record = mysql_fetch_row(results)))
    {
        name = record[0];
        wintimes = atoi(record[1]);
        losetimes = atoi(record[2]);
        score = atoi(record[3]);
        level = atoi(record[4]);
        xp = atoi(record[5]);
        head_id=atoi(record[6]);
        coins=atoi(record[7]);
        break;
    }
    mysql_free_result(results);
    return 0;
}

int CSampleDBPool::QueryPropNum(const int64&userid,std::vector<std::pair<int,int>>&v_prop)
{
    memset(m_szSQL, 0, MAX_SQL_LEN);
    int len = sprintf(m_szSQL, "SELECT prop_id,quantity from backpack where user_id = '%d'", userid);
    mcgWriteLog("CSampleDBPool::QueryPropNum sql : %s" , m_szSQL);
    
    int ret = mysql_real_query(m_hMySQL, m_szSQL, strlen(m_szSQL));
    if (ret != 0) {
        // 查询失败，处理错误
        mcgWriteLog("Error exec query: %s \n", mysql_error(m_hMySQL));
        return SQL_FAILED;
    }

    // 获取查询结果
    MYSQL_RES *results = mysql_store_result(m_hMySQL);
    if (!results) {
        // 获取结果集失败，处理错误
        mcgWriteLog("Error getting result set: %s \n", mysql_error(m_hMySQL));
        return SQL_FAILED;
    }

    // 遍历结果集
    MYSQL_ROW record;
    while ((record = mysql_fetch_row(results))) {
        // 将字段值转换为对应的数据类型并存储到各自的 vector 中
        int prop_id =atoi(record[0]);
        int quantity = atoi(record[1]);
        v_prop.push_back({prop_id,quantity});
    }
    mysql_free_result(results);
    
    return 0;
}




//获取排行榜
int CSampleDBPool::QueryRandingList(const int limit,const int offset, vector<int>& ids,vector<string>& accounts,vector<int>&wintimes,vector<int>&losetimes,vector<int>&scores,vector<int>&  levels,vector<int>&xps,vector<int>& head_ids)
{
    memset(m_szSQL, 0, MAX_SQL_LEN);
    int len = sprintf(m_szSQL, "SELECT id, username, win_times, lose_times, score, level, xp ,head FROM user ORDER BY score DESC , id ASC LIMIT %d OFFSET %d;", limit,offset);
    mcgWriteLog("CSampleDBPool::QueryRandingList sql : %s" , m_szSQL);
    
    int ret = mysql_real_query(m_hMySQL, m_szSQL, strlen(m_szSQL));
    if (ret != 0) {
        // 查询失败，处理错误
        mcgWriteLog("Error exec query: %s \n", mysql_error(m_hMySQL));
        return SQL_FAILED;
    }

    // 获取查询结果
    MYSQL_RES *results = mysql_store_result(m_hMySQL);
    if (!results) {
        // 获取结果集失败，处理错误
        mcgWriteLog("Error getting result set: %s \n", mysql_error(m_hMySQL));
        return SQL_FAILED;
    }

    // 遍历结果集
    MYSQL_ROW record;
    while ((record = mysql_fetch_row(results))) {
        // 将字段值转换为对应的数据类型并存储到各自的 vector 中
        ids.push_back(atoi(record[0]));
        accounts.push_back(record[1]);
        wintimes.push_back(atoi(record[2]));
        losetimes.push_back(atoi(record[3]));
        scores.push_back(atoi(record[4]));
        levels.push_back(atoi(record[5]));
        xps.push_back(atoi(record[6]));
        head_ids.push_back(atoi(record[7]));
    }
    mysql_free_result(results);
    return 0;
}

//更新用户信息
int CSampleDBPool::UpdateWinnerAndLoser(const int&winner_id, const int&loser_id,const int score,const int conis)
{
    if(score>0)
    {
        //非平局
        memset(m_szSQL, 0, MAX_SQL_LEN);
        int len = sprintf(m_szSQL, "update user set score = score + %d  ,coins = coins + %d where ID = %d;", score,conis,winner_id);
        mcgWriteLog("CSampleDBPool::UpdateWinnerAndLoser sql : %s" , m_szSQL);
        
        int ret = mysql_real_query(m_hMySQL, m_szSQL, strlen(m_szSQL));
        if (ret != 0) {
            // 查询失败，处理错误
            mcgWriteLog("Error exec query: %s \n", mysql_error(m_hMySQL));
            return SQL_FAILED;
        }
        
        memset(m_szSQL, 0, MAX_SQL_LEN);
        len = sprintf(m_szSQL, "update user set score = score - %d where ID = %d;", score,loser_id);
        mcgWriteLog("CSampleDBPool::UpdateWinnerAndLoser sql : %s" , m_szSQL);
        ret = mysql_real_query(m_hMySQL, m_szSQL, strlen(m_szSQL));
        if (ret != 0) {
            // 查询失败，处理错误
            mcgWriteLog("Error exec query: %s \n", mysql_error(m_hMySQL));
            return SQL_FAILED;
        }
        
        
        memset(m_szSQL, 0, MAX_SQL_LEN);
        len = sprintf(m_szSQL, "update user set win_times = win_times + 1 where ID = %d;", winner_id);
        mcgWriteLog("CSampleDBPool::UpdateWinnerAndLoser sql : %s" , m_szSQL);
        ret = mysql_real_query(m_hMySQL, m_szSQL, strlen(m_szSQL));
        if (ret != 0) {
            // 查询失败，处理错误
            mcgWriteLog("Error exec query: %s \n", mysql_error(m_hMySQL));
            return SQL_FAILED;
        }
        
        memset(m_szSQL, 0, MAX_SQL_LEN);
        len = sprintf(m_szSQL, "update user set lose_times = lose_times + 1 where ID = %d;", loser_id);
        mcgWriteLog("CSampleDBPool::UpdateWinnerAndLoser sql : %s" , m_szSQL);
        ret = mysql_real_query(m_hMySQL, m_szSQL, strlen(m_szSQL));
        if (ret != 0) {
            // 查询失败，处理错误
            mcgWriteLog("Error exec query: %s \n", mysql_error(m_hMySQL));
            return SQL_FAILED;
        }
    }
    else if(score == 0)
    {
        //平局
        memset(m_szSQL, 0, MAX_SQL_LEN);
        int len = sprintf(m_szSQL, "update user set lose_times = lose_times + 1 where ID = %d;", loser_id);
        mcgWriteLog("CSampleDBPool::UpdateWinnerAndLoser sql : %s" , m_szSQL);
        int ret = mysql_real_query(m_hMySQL, m_szSQL, strlen(m_szSQL));
        if (ret != 0) {
            // 查询失败，处理错误
            mcgWriteLog("Error exec query: %s \n", mysql_error(m_hMySQL));
            return SQL_FAILED;
        }
        
        memset(m_szSQL, 0, MAX_SQL_LEN);
        len = sprintf(m_szSQL, "update user set lose_times = lose_times + 1 where ID = %d;", winner_id);
        mcgWriteLog("CSampleDBPool::UpdateWinnerAndLoser sql : %s" , m_szSQL);
        ret = mysql_real_query(m_hMySQL, m_szSQL, strlen(m_szSQL));
        if (ret != 0) {
            // 查询失败，处理错误
            mcgWriteLog("Error exec query: %s \n", mysql_error(m_hMySQL));
            return SQL_FAILED;
        }
    }
    
    return 0;
}



/*
 CREATE TABLE `user` (
  `ID` bigint(20) NOT NULL AUTO_INCREMENT,
  `account` varchar(64) NOT NULL,
  `passwd`  varchar(256) NOT NULL,
  PRIMARY KEY (`account`),
  KEY `ID` (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=20000 DEFAULT CHARSET=utf8
 */
//插入用户账号信息
int CSampleDBPool::InsertUserInfo(const std::string& account , const std::string& passwd)
{
    memset(m_szSQL, 0, MAX_SQL_LEN);
    
    std::string username = "用户";
    // 获取当前时间点
    auto now = std::chrono::system_clock::now();
    // 将当前时间点转换为时间戳
    auto duration = now.time_since_epoch();
    auto timestamp = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
    // 将时间戳转换为纯数字字符串
    username += std::to_string(timestamp);
    
    
    int len = sprintf(m_szSQL, "insert into user(account , passwd ，username) values ('%s' , '%s','%s')", account.c_str() , passwd.c_str(),username.c_str());
    mcgWriteLog("CSampleDBPool::InsertUserInfo sql : %s" , m_szSQL);
    int ret = mysql_real_query(m_hMySQL, m_szSQL, len);
    if (IsNotZero(ret))
    {
        mcgWriteLog("Error exec query: %s \n", mysql_error(m_hMySQL));
        return SQL_FAILED;
    }
    return 0;
}

//修改用户信息
int CSampleDBPool::EditUserInfo(const int&user_id, int32 &headnum)
{
    memset(m_szSQL, 0, MAX_SQL_LEN);
    int len = sprintf(m_szSQL, "update user set head=%d where ID =%d",headnum, user_id);
    mcgWriteLog("CSampleDBPool::EditUserInfo sql : %s" , m_szSQL);
    int ret = mysql_real_query(m_hMySQL, m_szSQL, len);
    if (IsNotZero(ret))
    {
        mcgWriteLog("Error exec query: %s \n", mysql_error(m_hMySQL));
        return SQL_FAILED;
    }
    return 0;
}

int CSampleDBPool::EditUserInfo(const int&user_id, std::string &username)
{
    memset(m_szSQL, 0, MAX_SQL_LEN);
    int len = sprintf(m_szSQL, "update user set username='%s' where ID =%d",username.c_str(), user_id);
    mcgWriteLog("CSampleDBPool::EditUserInfo sql : %s" , m_szSQL);
    int ret = mysql_real_query(m_hMySQL, m_szSQL, len);
    if (IsNotZero(ret))
    {
        mcgWriteLog("Error exec query: %s \n", mysql_error(m_hMySQL));
        return SQL_FAILED;
    }
    return 0;
}



void PositionToSring(const REQGameOver&req,std::string&black_postions_str,std::string&white_positions_str)
{
    for (int i = 0; i < req.black_positions_size(); ++i)
    {
        const Position& position = req.black_positions(i);
        string str = "(" + std::to_string(position.row()) + "," + std::to_string(position.col())+ ")";
        black_postions_str +=str;
    }
    for (int i = 0; i < req.white_positions_size(); ++i)
    {
        const Position& position = req.white_positions(i);
        string str = "(" + std::to_string(position.row()) + "," + std::to_string(position.col())+ ")";
        white_positions_str +=str;
    }
}

int CSampleDBPool::UpdateGameInfo(const REQGameOver&req)
{
    memset(m_szSQL, 0, MAX_SQL_LEN);
    
    std::string black_positions_str;
    std::string white_positions_str;
    PositionToSring(req,black_positions_str,white_positions_str);
    
    int len = sprintf(m_szSQL, "insert into game_info(winner_id,loser_id,score,black_id,white_id,is_no_winner,is_force_exit,black_positions,white_positions) values(%d,%d,%d,%d,%d,%d,%d,'%s','%s');", req.winner_id(),req.loser_id(),req.score(),req.black_id(),req.white_id(),req.is_no_winner(),req.is_force_exit(),black_positions_str.c_str(),white_positions_str.c_str());
    mcgWriteLog("CSampleDBPool::UpdateGameInfo sql : %s" , m_szSQL);
    int ret = mysql_real_query(m_hMySQL, m_szSQL, len);
    if (IsNotZero(ret))
    {
        mcgWriteLog("Error exec query: %s \n", mysql_error(m_hMySQL));
        return SQL_FAILED;
    }
    
    return 0;
}


void CSampleDBPool::StringTOPosition(std::vector<Position>&positions,std::string& s)
{
    printf("要反序列化的position_str:%s\n",s.c_str());
    size_t pos = 0;
    while (pos < s.length()) {
        // Skip to the next '(' character
        pos = s.find('(', pos);
        if (pos == std::string::npos) {
            break;
        }

        // Parse the two integers between the parentheses
        int row, col;
        if (sscanf(&s[pos], "(%d,%d)", &row, &col) != 2) {
            break;
        }
        Position position;
        position.set_row(row);
        position.set_col(col);
        positions.push_back(position);

        // Advance to the next position after the closing ')' character
        pos = s.find(')', pos);
        if (pos == std::string::npos) {
            break;
        }
        ++pos;
    }

}

int CSampleDBPool::GetGameHistory(int player_id,int limit,int offset,std::vector<GameInfo>&game_infos)
{
    memset(m_szSQL, 0, MAX_SQL_LEN);
    
    int len = sprintf(m_szSQL, "SELECT id, winner_id, loser_id, score, black_id, white_id, is_no_winner ,is_force_exit,black_positions,white_positions,time FROM game_info  where winner_id = %d OR loser_id = %d ORDER BY `time` DESC LIMIT %d OFFSET %d;", player_id,player_id,limit,offset);
    mcgWriteLog("CSampleDBPool::GetGameHistory sql : %s" , m_szSQL);
    
    int ret = mysql_real_query(m_hMySQL, m_szSQL, strlen(m_szSQL));
    if (ret != 0) {
        // 查询失败，处理错误
        mcgWriteLog("Error exec query: %s \n", mysql_error(m_hMySQL));
        return SQL_FAILED;
    }

    // 获取查询结果
    MYSQL_RES *results = mysql_store_result(m_hMySQL);
    if (!results) {
        // 获取结果集失败，处理错误
        mcgWriteLog("Error getting result set: %s \n", mysql_error(m_hMySQL));
        return SQL_FAILED;
    }

    
    
    // 遍历结果集
    MYSQL_ROW record;
    while ((record = mysql_fetch_row(results))) {
        // 将字段值转换为对应的数据类型并存储到各自的 vector 中
        GameInfo game_info;
        game_info.set_game_id(atoi(record[0]));
        game_info.set_winner_id(atoi(record[1]));
        game_info.set_loser_id(atoi(record[2]));
        game_info.set_score(atoi(record[3]));
        game_info.set_black_id(atoi(record[4]));
        game_info.set_white_id(atoi(record[5]));
        game_info.set_is_no_winner(atoi(record[6]));
        game_info.set_is_force_exit(atoi(record[7]));
        
        std::string black_position_str = record[8];
        std::vector<Position> black_positions;
        StringTOPosition(black_positions,black_position_str);
        for (size_t i = 0; i < black_positions.size(); ++i)
        {
            Position *position = game_info.add_black_positions();
            position->set_row(black_positions[i].row());
            position->set_col(black_positions[i].col());
        }
        
        std::string white_position_str = record[9];
        std::vector<Position> white_positions;
        StringTOPosition(white_positions,white_position_str);
        for (size_t i = 0; i < white_positions.size(); ++i)
        {
            Position *position = game_info.add_white_positions();
            position->set_row(white_positions[i].row());
            position->set_col(white_positions[i].col());
        }
        game_info.set_game_time(record[10]);

        
        
        //获取对手用户信息
        int64 enemy_id = (player_id == game_info.black_id() ? game_info.white_id() : game_info.black_id());
        std::string username;
        int32 wintimes=0;
        int32 losetimes=0;
        int32 score=0;
        int32 level=0;
        int32 xp=0;
        int32 head_id=0;
        int ret =QueryUserAllInfo(enemy_id,username,wintimes,losetimes,score,level,xp,head_id);
        if(IsNotZero(ret))
        {
            printf("debug+++获取用户信息失败 id:%d",enemy_id);
        }
        UserInfo userInfo;

       // 设置 UserInfo 中的字段值
       userInfo.set_id(enemy_id);
       userInfo.set_username(username);
       userInfo.set_wintimes(wintimes);
       userInfo.set_losetimes(losetimes);
       userInfo.set_score(score);
       userInfo.set_level(level);
       userInfo.set_xp(xp);
       userInfo.set_head_id(head_id);
        game_info.mutable_enemy_userinfo()->CopyFrom(userInfo);
        game_infos.push_back(game_info);
    }
    mysql_free_result(results);
    
    return 0;
}




int CSampleDBPool::QueryUserAllInfo(const std::string& account ,int32&id,int32&wintimes,int32&losetimes,int32&score,int& level,int32&xp,int32&head_id,std::string&username)
{
    memset(m_szSQL, 0, MAX_SQL_LEN);
    int len = sprintf(m_szSQL, "select ID,win_times, lose_times, score, level, XP,head ,username from user where account = '%s'", account.c_str());
    mcgWriteLog("CSampleDBPool::QueryUserAllInfo sql : %s" , m_szSQL);
    int ret = mysql_real_query(m_hMySQL, m_szSQL, len);
    if (IsNotZero(ret))
    {
        mcgWriteLog("Error exec query: %s \n", mysql_error(m_hMySQL));
        return SQL_FAILED;
    }
    MYSQL_RES *results = mysql_store_result(m_hMySQL);
    MYSQL_ROW record;
    while((record = mysql_fetch_row(results)))
    {
        id = atoi(record[0]);
        wintimes = atoi(record[1]);
        losetimes = atoi(record[2]);
        score = atoi(record[3]);
        level = atoi(record[4]);
        xp = atoi(record[5]);
        head_id=atoi(record[6]);
        username=record[7];
        break;
    }
    mysql_free_result(results);
    return 0;
}

int CSampleDBPool::QueryIsFriend(int friend_id,int user_id, bool&is_friend)
{
    memset(m_szSQL, 0, MAX_SQL_LEN);
    int len = sprintf(m_szSQL, "select * from friends where ( user_id = %d AND friend_id = %d ) OR ( user_id = %d AND friend_id = %d )", user_id,friend_id,friend_id,user_id);
    mcgWriteLog("CSampleDBPool::QueryIsFriend sql : %s" , m_szSQL);
    int ret = mysql_real_query(m_hMySQL, m_szSQL, len);
    if (IsNotZero(ret))
    {
        mcgWriteLog("Error exec query: %s \n", mysql_error(m_hMySQL));
        return SQL_FAILED;
    }
    MYSQL_RES *results = mysql_store_result(m_hMySQL);
    MYSQL_ROW record;
    
    //判断结果集是否为1
    int num_rows = mysql_num_rows(results);
    if (num_rows == 1)
    {
        // 只有一条数据
        is_friend = true;
    }
    else if(num_rows == 0)
    {
        is_friend = false;
    }
    else
    {
        mcgWriteLog("查出了: %d 条数据，error!" , num_rows);
        return SQL_FAILED;
    }
    mysql_free_result(results);
    return 0;
}



int CSampleDBPool::AddWaitFriend(int friend_id,int user_id)
{
    int len = sprintf(m_szSQL, "insert into wait_friend(user_id,friend_id) values(%d,%d);", friend_id,user_id);
    mcgWriteLog("CSampleDBPool::AddWaitFriend sql : %s" , m_szSQL);
    int ret = mysql_real_query(m_hMySQL, m_szSQL, len);
    if (IsNotZero(ret))
    {
        mcgWriteLog("Error exec query: %s \n", mysql_error(m_hMySQL));
        return SQL_FAILED;
    }
    
    return 0;
}


int CSampleDBPool::FindAddWaitFriendNum(int friend_id,int&friend_num)
{
    memset(m_szSQL, 0, MAX_SQL_LEN);
    int len = sprintf(m_szSQL, "select * from wait_friend where user_id = %d", friend_id);
    mcgWriteLog("CSampleDBPool::FindAddWaitFriendNum sql : %s" , m_szSQL);
    int ret = mysql_real_query(m_hMySQL, m_szSQL, len);
    if (IsNotZero(ret))
    {
        mcgWriteLog("Error exec query: %s \n", mysql_error(m_hMySQL));
        return SQL_FAILED;
    }
    MYSQL_RES *results = mysql_store_result(m_hMySQL);
    
    //判断结果集是否为1
    friend_num = mysql_num_rows(results);
    mysql_free_result(results);
    return 0;
}

int CSampleDBPool::RemoveWaitFriend(int user_id,int friend_id)
{
    memset(m_szSQL, 0, MAX_SQL_LEN);
    int len = sprintf(m_szSQL, "delete from wait_friend where user_id = %d and friend_id = %d", user_id,friend_id);
    mcgWriteLog("CSampleDBPool::RemoveWaitFriend sql : %s" , m_szSQL);
    int ret = mysql_real_query(m_hMySQL, m_szSQL, len);
    if (IsNotZero(ret))
    {
        mcgWriteLog("Error exec query: %s \n", mysql_error(m_hMySQL));
        return SQL_FAILED;
    }
    MYSQL_RES *results = mysql_store_result(m_hMySQL);
    
    mysql_free_result(results);
    return 0;
}
int CSampleDBPool::AddFriend(int user_id,int friend_id)
{
    int len = sprintf(m_szSQL, "insert into friends(user_id,friend_id) values(%d,%d);", user_id,friend_id);
    mcgWriteLog("CSampleDBPool::AddFriend sql : %s" , m_szSQL);
    int ret = mysql_real_query(m_hMySQL, m_szSQL, len);
    if (IsNotZero(ret))
    {
        mcgWriteLog("Error exec query: %s \n", mysql_error(m_hMySQL));
        return SQL_FAILED;
    }
    
    return 0;
}
int CSampleDBPool::GetFriendIDs(int user_id,std::vector<int>&friend_ids)
{
    memset(m_szSQL, 0, MAX_SQL_LEN);
    int len = sprintf(m_szSQL, "select user_id from friends where friend_id = %d ;", user_id);
    mcgWriteLog("CSampleDBPool::GetFriendIDs sql : %s" , m_szSQL);
    
    int ret = mysql_real_query(m_hMySQL, m_szSQL, strlen(m_szSQL));
    if (ret != 0) {
        // 查询失败，处理错误
        mcgWriteLog("Error exec query: %s \n", mysql_error(m_hMySQL));
        return SQL_FAILED;
    }

    // 获取查询结果
    MYSQL_RES *results = mysql_store_result(m_hMySQL);
    if (!results) {
        // 获取结果集失败，处理错误
        mcgWriteLog("Error getting result set: %s \n", mysql_error(m_hMySQL));
        return SQL_FAILED;
    }
    // 遍历结果集
    MYSQL_ROW record;
    while ((record = mysql_fetch_row(results))) {
        friend_ids.push_back(atoi(record[0]));
    }
    mysql_free_result(results);
    
    memset(m_szSQL, 0, MAX_SQL_LEN);
    
    len = sprintf(m_szSQL, "select friend_id from friends where user_id = %d ;", user_id);
    mcgWriteLog("CSampleDBPool::GetFriendIDs sql : %s" , m_szSQL);
    ret = mysql_real_query(m_hMySQL, m_szSQL, strlen(m_szSQL));
    if (ret != 0) {
        // 查询失败，处理错误
        mcgWriteLog("Error exec query: %s \n", mysql_error(m_hMySQL));
        return SQL_FAILED;
    }

    // 获取查询结果
    results = mysql_store_result(m_hMySQL);
    if (!results) {
        // 获取结果集失败，处理错误
        mcgWriteLog("Error getting result set: %s \n", mysql_error(m_hMySQL));
        return SQL_FAILED;
    }
    // 遍历结果集
    while ((record = mysql_fetch_row(results))) {
        friend_ids.push_back(atoi(record[0]));
    }
    mysql_free_result(results);
    
    return 0;
}

int CSampleDBPool::GetWaitFriendIDs(int user_id,std::vector<int>&wait_friend_ids,std::vector<std::string>&times)
{
    memset(m_szSQL, 0, MAX_SQL_LEN);
    int len = sprintf(m_szSQL, "select friend_id ,time from wait_friend where user_id = %d ;", user_id);
    mcgWriteLog("CSampleDBPool::GetFriendIDs sql : %s" , m_szSQL);
    
    int ret = mysql_real_query(m_hMySQL, m_szSQL, strlen(m_szSQL));
    if (ret != 0) {
        // 查询失败，处理错误
        mcgWriteLog("Error exec query: %s \n", mysql_error(m_hMySQL));
        return SQL_FAILED;
    }

    // 获取查询结果
    MYSQL_RES *results = mysql_store_result(m_hMySQL);
    if (!results) {
        // 获取结果集失败，处理错误
        mcgWriteLog("Error getting result set: %s \n", mysql_error(m_hMySQL));
        return SQL_FAILED;
    }
    // 遍历结果集
    MYSQL_ROW record;
    while ((record = mysql_fetch_row(results))) {
        wait_friend_ids.push_back(atoi(record[0]));
        times.push_back(record[1]);
    }
    mysql_free_result(results);
    return 0;
}

int CSampleDBPool::DeleteFriend(int user_id,int friend_id)
{
    memset(m_szSQL, 0, MAX_SQL_LEN);
    int len = sprintf(m_szSQL, "delete from friends where (user_id = %d and friend_id =%d) or (user_id = %d and friend_id=%d);", user_id,friend_id,friend_id,user_id);
    mcgWriteLog("CSampleDBPool::DeleteFriend sql : %s" , m_szSQL);
    mcgWriteLog("CSampleDBPool::RemoveWaitFriend sql : %s" , m_szSQL);
    int ret = mysql_real_query(m_hMySQL, m_szSQL, len);
    if (IsNotZero(ret))
    {
        mcgWriteLog("Error exec query: %s \n", mysql_error(m_hMySQL));
        return SQL_FAILED;
    }
    MYSQL_RES *results = mysql_store_result(m_hMySQL);
    
    mysql_free_result(results);
    return 0;
}


//给用户减少金币
int CSampleDBPool::ReducedCoins(int user_id,int coins_num)
{
    memset(m_szSQL, 0, MAX_SQL_LEN);
    int len = sprintf(m_szSQL, "update user set coins= coins - %d where ID= %d;", coins_num,user_id);
    mcgWriteLog("CSampleDBPool::ReducedCoins sql : %s" , m_szSQL);
    int ret = mysql_real_query(m_hMySQL, m_szSQL, len);
    if (IsNotZero(ret))
    {
        mcgWriteLog("Error exec query: %s \n", mysql_error(m_hMySQL));
        return SQL_FAILED;
    }
    MYSQL_RES *results = mysql_store_result(m_hMySQL);
    
    mysql_free_result(results);
    return 0;
}

//判断用户是否购买过这个道具
int CSampleDBPool::CheckUserHasItem(int user_id,int item_id,bool&is_has)
{
    memset(m_szSQL, 0, MAX_SQL_LEN);
    int len = sprintf(m_szSQL, "select * from backpack where user_id = %d AND prop_id = %d", user_id,item_id);
    mcgWriteLog("CSampleDBPool::CheckUserHasItem sql : %s" , m_szSQL);
    int ret = mysql_real_query(m_hMySQL, m_szSQL, len);
    if (IsNotZero(ret))
    {
        mcgWriteLog("Error exec query: %s \n", mysql_error(m_hMySQL));
        return SQL_FAILED;
    }
    MYSQL_RES *results = mysql_store_result(m_hMySQL);
    MYSQL_ROW record;
    
    //判断结果集是否为1
    int num_rows = mysql_num_rows(results);
    if (num_rows == 1)
    {
        // 只有一条数据
        is_has = true;
    }
    else if(num_rows == 0)
    {
        is_has = false;
    }
    else
    {
        mcgWriteLog("查出了: %d 条数据，error!" , num_rows);
        return SQL_FAILED;
    }
    mysql_free_result(results);
    return 0;
}
//用户没有购买过这个道具，增加这个道具以及数量
int CSampleDBPool::AddItem(int user_id,int item_id,int num)
{
    int len = sprintf(m_szSQL, "insert into backpack(user_id,prop_id,quantity) values(%d,%d,%d);", user_id,item_id,num);
    mcgWriteLog("CSampleDBPool::AddItem sql : %s" , m_szSQL);
    int ret = mysql_real_query(m_hMySQL, m_szSQL, len);
    if (IsNotZero(ret))
    {
        mcgWriteLog("Error exec query: %s \n", mysql_error(m_hMySQL));
        return SQL_FAILED;
    }
    return 0;
}
//用户有这个道具，给用户增加道具数量
int CSampleDBPool::IncreaseItemNum(int user_id,int item_id,int num)
{
    memset(m_szSQL, 0, MAX_SQL_LEN);
    int len = sprintf(m_szSQL, "update backpack set quantity= quantity + %d where user_id= %d and prop_id = %d;", num,user_id,item_id);
    mcgWriteLog("CSampleDBPool::IncreaseItemNum sql : %s" , m_szSQL);
    int ret = mysql_real_query(m_hMySQL, m_szSQL, len);
    if (IsNotZero(ret))
    {
        mcgWriteLog("Error exec query: %s \n", mysql_error(m_hMySQL));
        return SQL_FAILED;
    }
    MYSQL_RES *results = mysql_store_result(m_hMySQL);
    
    mysql_free_result(results);
    return 0;
}


int CSampleDBPool::ReduceItemNum(int user_id,int item_id,int num)
{
    memset(m_szSQL, 0, MAX_SQL_LEN);
    int len = sprintf(m_szSQL, "update backpack set quantity= quantity - %d where user_id= %d and prop_id = %d;", num,user_id,item_id);
    mcgWriteLog("CSampleDBPool::ReduceItemNum sql : %s" , m_szSQL);
    int ret = mysql_real_query(m_hMySQL, m_szSQL, len);
    if (IsNotZero(ret))
    {
        mcgWriteLog("Error exec query: %s \n", mysql_error(m_hMySQL));
        return SQL_FAILED;
    }
    MYSQL_RES *results = mysql_store_result(m_hMySQL);
    
    mysql_free_result(results);
    return 0;
}

int CSampleDBPool::GetItemNum(int user_id,int item_id,int&num)
{
 
    memset(m_szSQL, 0, MAX_SQL_LEN);
    int len = sprintf(m_szSQL, "select quantity from backpack where user_id = %d AND prop_id = %d", user_id,item_id);
    mcgWriteLog("CSampleDBPool::CheckUserHasItem sql : %s" , m_szSQL);
    int ret = mysql_real_query(m_hMySQL, m_szSQL, len);
    if (IsNotZero(ret))
    {
        mcgWriteLog("Error exec query: %s \n", mysql_error(m_hMySQL));
        return SQL_FAILED;
    }
    MYSQL_RES *results = mysql_store_result(m_hMySQL);
    MYSQL_ROW record;
    while((record = mysql_fetch_row(results)))
    {
        num = atoi(record[0]);
        break;
    }
    return 0;
}

int CSampleDBPool::ClearHistoryOutTime(std::string time)
{
    memset(m_szSQL, 0, MAX_SQL_LEN);
    int len = sprintf(m_szSQL, "DELETE FROM game_info WHERE time < '%s'",time.c_str());
    mcgWriteLog("CSampleDBPool::ClearHistoryOutTime sql : %s" , m_szSQL);
    int ret = mysql_real_query(m_hMySQL, m_szSQL, len);
    if (IsNotZero(ret))
    {
        mcgWriteLog("Error exec query: %s \n", mysql_error(m_hMySQL));
        return SQL_FAILED;
    }

    return 0;
}
