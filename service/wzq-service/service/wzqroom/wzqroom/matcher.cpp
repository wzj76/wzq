//
//  matcher.cpp
//  wzqroom
//
//  Created by duole on 2024/2/18.
//

#include "matcher.hpp"
#include "ProtocolDef.h"
#include "wzqProtocol.pb.h"
MathchQueue::MathchQueue(){}
MathchQueue::~MathchQueue(){}

//获取元素个数
size_t MathchQueue::size()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    return m_list.size();
}

//判断是否为空
bool MathchQueue::empty()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    return m_list.empty();
}

//等待
void MathchQueue::wait()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_cond.wait(lock); //等待
}

//唤醒
void MathchQueue::wakeup()
{
    m_cond.notify_all();
}

//入队
void MathchQueue::push(const int& in)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_list.push_back(in);
    wakeup();
}

//出队
bool MathchQueue::pop(int* out)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    if(m_list.empty())
    {
      return false;
    }
    *out = m_list.front();
    m_list.pop_front();
    return true;
}

//移除队列数据
void MathchQueue::remove(int& data)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_list.remove(data);
}


    


void Matcher::HandlMatch(MathchQueue& mq)
{
    while(true)
    {
        //1.队列人数是否小于2，阻塞等待
        while(mq.size() < 2)
        {
            mq.wait();
        }

        //2.从队列中抽取2个玩家
        int id1,id2;
        bool ret = mq.pop(&id1);
        if(ret == false)
        {
            //玩家1 出队失败
            continue;
        }

        ret = mq.pop(&id2);
        if(ret == false)
        {
            //玩家2出队失败
            mq.push(id1);//把玩家1重新放回队列
            continue;
        }

        //检查这两个人的信息是否存在
        if(m_umPlayerInfoDeque.find(id1)==m_umPlayerInfoDeque.end())
        {
            continue;
        }
        if(m_umPlayerInfoDeque.find(id2)==m_umPlayerInfoDeque.end())
        {
            continue;
        }
        //找到两个人的信息，进行匹配
        PlayerInfo playerInfo1 = m_umPlayerInfoDeque[id1];
        PlayerInfo playerInfo2 = m_umPlayerInfoDeque[id2];

        //随机选择一个人成为黑棋
        int black_id = 0;
        int white_id = 0;
        if(std::rand()%2 ==0)
        {
            black_id = playerInfo1.m_userid;
            white_id = playerInfo2.m_userid;
        }
        else
        {
            black_id = playerInfo2.m_userid;
            white_id = playerInfo1.m_userid;
        }
        
        SendMatchMessage(playerInfo1,playerInfo2,black_id,white_id);
        SendMatchMessage(playerInfo2,playerInfo1,black_id,white_id);
        
        //需要向game服务器发送消息创建房间
        SendCreateGameMessage(playerInfo1,playerInfo2,black_id,white_id);
        
        std::unique_lock<std::mutex> lock(m_mutex);
        m_umPlayerInfoDeque.erase(id1);
        m_umPlayerInfoDeque.erase(id2);
    }
}
void Matcher::SendMatchMessage(PlayerInfo& cur_user,PlayerInfo& enemy_user,int black_id,int white_id)
{
    ACKRoom ack;
    UserInfo pb_userinfo = enemy_user.m_userinfo;
    ack.mutable_userinfo()->CopyFrom(pb_userinfo);
    ack.set_code(0);
    ack.set_result_msg("ack_match_sucess");
    
    
    ack.set_black_id(black_id);
    ack.set_white_id(white_id);
    printf("发送匹配成功的ack\n");
    m_room_service->SendSvrdMsg(m_room_service->GetConnSvrdMsgRoute(), (MSGID_MATCH | ID_ACK), 0, 0, ack.SerializeAsString(), cur_user.m_clientid);
    
}

void Matcher::SendCreateGameMessage(PlayerInfo& cur_user,PlayerInfo& enemy_user,int black_id,int white_id)
{

    REQCreatGame req;
    req.set_req_msg("create_game");
    req.mutable_userinfo_player1()->CopyFrom(cur_user.m_userinfo);
    req.set_player1_clientid(cur_user.m_clientid);
    req.mutable_userinfo_player2()->CopyFrom(enemy_user.m_userinfo);
    req.set_player2_clientid(enemy_user.m_clientid);
    req.set_black_id(black_id);
    req.set_white_id(white_id);
    req.set_board_row(13);
    req.set_board_col(13);
    

    printf("发送创建房间的req\n");
    m_room_service->SendSvrdMsg(m_room_service->GetGameSvrdMsgRoute(), (MSGID_CREATE_GAME | ID_REQ), 0, 0, req.SerializeAsString(), 0);
    
}


void Matcher::ThreadLevel1Entry()
{
  HandlMatch(m_level_1);
}
void Matcher::ThreadLevel2Entry()
{
  HandlMatch(m_level_2);
}


Matcher::Matcher()
    : m_th_level_1(std::thread(&Matcher::ThreadLevel1Entry,this))
    ,m_th_level_2(std::thread(&Matcher::ThreadLevel2Entry,this))
    {
    }

Matcher::Matcher(CRoomService* room_service)
    : m_th_level_1(std::thread(&Matcher::ThreadLevel1Entry,this))
    ,m_th_level_2(std::thread(&Matcher::ThreadLevel2Entry,this))
    , m_room_service(room_service)

    {
    }
Matcher::~Matcher(){}

bool Matcher::AddUser(int32 id,PlayerInfo& info)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_umPlayerInfoDeque[id] = info;
    
    if(info.m_userinfo.score()<=3000)
    {
        m_level_1.push(id);
    }
    else if(info.m_userinfo.score()>3000)
    {
        m_level_2.push(id);
    }
    
    return true;
}

bool Matcher::DelUser(int32 id)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    
    PlayerInfo info = m_umPlayerInfoDeque[id];
    m_umPlayerInfoDeque.erase(id);
    
    if(info.m_userinfo.score()<=3000)
    {
        m_level_1.remove(id);
    }
    else if(info.m_userinfo.score()>3000)
    {
        m_level_2.remove(id);
    }
    
    return true;
}

