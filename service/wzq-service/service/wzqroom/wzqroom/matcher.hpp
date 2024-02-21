//
//  matcher.hpp
//  wzqroom
//
//  Created by duole on 2024/2/18.
//

#ifndef matcher_hpp
#define matcher_hpp

#include <stdio.h>
#include <list>
#include <memory>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <unordered_map>
#include "PlayerInfo.hpp"
#include "RoomService.h"
class MathchQueue
{
private:
    std::list<int> m_list;  //阻塞队列
    std::mutex m_mutex;  //互斥锁
    std::condition_variable m_cond; //条件变量，消费者<2时等待

public:
    MathchQueue();
    ~MathchQueue();

  //获取元素个数
    size_t size();

  //判断是否为空
    bool empty();

  //等待
    void wait();
  
  //唤醒
    void wakeup();

  //入队
    void push(const int& in);

  //出队
    bool pop(int* out);

  //移除队列数据
    void remove(int& data);
};

class CRoomService;
class Matcher
{
private:
    MathchQueue m_level_1;  //匹配队列1，，3000分以下使用
    MathchQueue m_level_2;  //匹配队列2，，3000分以上使用
    
    std::thread m_th_level_1;
    std::thread m_th_level_2;
    std::unordered_map<int32,PlayerInfo>              m_umPlayerInfoDeque       ;  //用于保存匹配用户信息
    std::mutex m_mutex;  //互斥锁
    CRoomService* m_room_service;
    
private:

    void HandlMatch(MathchQueue& mq);
    void ThreadLevel1Entry();
    void ThreadLevel2Entry();
public:
    Matcher();
    Matcher(CRoomService* room_service);
    ~Matcher();

    bool AddUser(int32 id,PlayerInfo& info);

    bool DelUser(int32 id);
    void SendMatchMessage(PlayerInfo& cur_user,PlayerInfo& enemy_user,int black_id,int white_id);
    void SendCreateGameMessage(PlayerInfo& cur_user,PlayerInfo& enemy_user,int black_id,int white_id);
};

#endif /* matcher_hpp */
