//
//  RandingList.cpp
//  wzqauth
//
//  Created by duole on 2024/2/19.
//

#include "RandingList.hpp"
#include <chrono>
#include "sampledbpool.h"

RandingList::RandingList(CAuthService* authService)
    :m_pAuthService(authService)
{
    
}
RandingList::~RandingList()
{
    
}

void RandingList::StartRandingListTimer()
{
    //启动时，先获取一次排行榜
    AutoGetRandingList(this,0);
    AutoClearHistory(this, 0);
    
    // 获取当前时间
    auto now = std::chrono::system_clock::now();
    time_t tt = std::chrono::system_clock::to_time_t(now);
    tm local_tm = *localtime(&tt);

    // 计算距离第二天零点的时间间隔
    int seconds_until_midnight = 24 * 60 * 60 - (local_tm.tm_hour * 60 * 60 + local_tm.tm_min * 60 + local_tm.tm_sec);
    
    // 创建定时器，设置初始触发时间和触发间隔，每天凌晨触发
    m_randingListTimer = CMcgTimer::CreateTimer(AutoGetRandingList, this, seconds_until_midnight * 1000, 24 * 60 * 60 * 1000);
    m_clearHistoryTimer = CMcgTimer::CreateTimer(AutoClearHistory, this, seconds_until_midnight * 1000, 24 * 60 * 60 * 1000);
}
void* RandingList::AutoGetRandingList(void* ptr,unsigned int a)
{
    RandingList* p_randingList = static_cast<RandingList*>(ptr);
    p_randingList->m_randingList.clear();
    std::vector<int> ids;
    std::vector<std::string> usernames;
    std::vector<int> wintimes;
    std::vector<int> losetimes;
    std::vector<int> scores;
    std::vector<int> levels;
    std::vector<int> xps;
    std::vector<int> head_ids;
    int nresult = p_randingList->m_pAuthService->m_pSampleDBPool->QueryRandingList(100,0,ids,usernames,wintimes,losetimes,scores,levels,xps,head_ids);

    std::unique_lock<std::mutex>(p_randingList->m_mutex);
    //将数组中的数据放到ack中
    for (size_t i = 0; i < ids.size(); ++i)
    {
        UserInfo userInfo;
        userInfo.set_id(ids[i]);
        userInfo.set_username(usernames[i]);
        userInfo.set_wintimes(wintimes[i]);
        userInfo.set_losetimes(losetimes[i]);
        userInfo.set_score(scores[i]);
        userInfo.set_level(levels[i]);
        userInfo.set_xp(xps[i]);
        userInfo.set_head_id(head_ids[i]);
        p_randingList->m_randingList.push_back(userInfo);
    }
    
    return nullptr;
}

void RandingList::SetRandingList(int limit,int offset,std::vector<UserInfo>&out_userinfos)
{
    std::unique_lock<std::mutex>(m_mutex);
    if(offset>m_randingList.size())
    {
        offset = m_randingList.size();
    }
    if(offset + limit >m_randingList.size())
    {
        limit = 0;
    }
    for(int i=offset;i<m_randingList.size()&&i<offset+limit;i++)
    {
        out_userinfos.push_back(m_randingList[i]);
    }
}

void* RandingList::AutoClearHistory(void* ptr,unsigned int a)
{
    RandingList* p_randingList = static_cast<RandingList*>(ptr);
    
    time_t now = time(nullptr);
    struct tm thirty_days_ago_tm = *localtime(&now);
    thirty_days_ago_tm.tm_mday -= 30;
    time_t thirty_days_ago = mktime(&thirty_days_ago_tm);

    char formatted_time[20];
       strftime(formatted_time, sizeof(formatted_time), "%Y-%m-%d %H:%M:%S", localtime(&thirty_days_ago));
    
    int nresult =p_randingList->m_pAuthService->m_pSampleDBPool->ClearHistoryOutTime(formatted_time);
    
    printf("清理过期对局完成,返回值:%d\n",nresult);
    return  nullptr;
}
