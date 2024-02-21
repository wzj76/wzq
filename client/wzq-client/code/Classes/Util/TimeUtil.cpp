#include "TimeUtil.h"
#include "MacroUtil.h"

USING_NS_CC;
using namespace std;

// 忘记调用Start的提醒信息
const string kTipForgetToCallStartMsg = "may forgot to call Start() first.";

namespace util
{
    
    TimeUtil::TimeUtil():begin_time_(kInvalidValue)
    {
    }
    
    void TimeUtil::Start()
    {
        begin_time_ = MillisecondToNow();
        pause_resume_note_.clear();  // 重置记录
    }
    
    void TimeUtil::Pause()
    {
        // 确保调用Start了
        CCAssert(begin_time_ != kInvalidValue, kTipForgetToCallStartMsg.c_str());
        // 确保没有重复调用Pause， 比如Pause后没有Resume或End然后又Pause了
        CCAssert(pause_resume_note_.empty() || pause_resume_note_.back().second != kInvalidValue,
                 "may continue call Pause() 2 times without call Resume().");
        int cur_time = MillisecondToNow();
        pause_resume_note_.push_back(make_pair(cur_time, kInvalidValue));
    }
    
    void TimeUtil::Resume()
    {
        // 确保调用Start了
        CCAssert(begin_time_ != kInvalidValue, kTipForgetToCallStartMsg.c_str());
        // 确保没有还未Pause就Resume，确保没有重复调用Resume（比如Resume后又Resume了）
        CCAssert(!pause_resume_note_.empty() && pause_resume_note_.back().second == kInvalidValue, "");
        pause_resume_note_.back().second = MillisecondToNow();
    }
    
    void TimeUtil::Restart()
    {
        Start();
    }
    
    pair<int, string> TimeUtil::CalculateSpentTime()
    {
        // 确保调用Start了
        CCAssert(begin_time_ != kInvalidValue, kTipForgetToCallStartMsg.c_str());
        // 确保上次调用的是Pause
        CCAssert(!pause_resume_note_.empty() && pause_resume_note_.back().second == kInvalidValue, "");
        const int kTotalSpentTime = pause_resume_note_.back().first - begin_time_;
        int total_pause_time = 0;
        for (int i = 0; i < pause_resume_note_.size() - 1; i++)
        {
            total_pause_time += pause_resume_note_[i].second - pause_resume_note_[i].first;
        }
        const int kRealSpentTime = kTotalSpentTime - total_pause_time;
        return make_pair(kRealSpentTime, GetHHMMSSStrFromMillisecond(kRealSpentTime));
    }
    
    long long TimeUtil::MillisecondToNow()
    {
        struct timeval now;
        gettimeofday(&now, nullptr);
        long long nowSec = now.tv_sec;
        long long nowMic = nowSec * 1000;
        nowMic += now.tv_usec / 1000;
        
        return nowMic;
    }
    long TimeUtil::SecondToNow()
    {
        time_t now;
        time(&now);
        return (long)now;
    }
    string TimeUtil::GetHHMMSSStrFromMillisecond(long spent_time)
    {
        CCAssert(spent_time >= 0, "");
        long millisecond = spent_time / 1000;
        const long kHour = millisecond / (60 * 60);
        millisecond -= kHour * (60 * 60);
        const long kMinute = millisecond / 60;
        millisecond -= kMinute * 60;
        const long kSecond = millisecond;
        return str_format("%02ld:%02ld:%02ld", kHour, kMinute, kSecond);
    }
    
    string TimeUtil::GetHHMMSSStrFromSecond(long spent_time)
    {
        return GetHHMMSSStrFromMillisecond(spent_time * 1000);
    }
    
    string TimeUtil::GetMMSSStrFromMillisecond(long spent_time)
    {
        CCAssert(spent_time >= 0, "");
        long millisecond = spent_time / 1000;
        const long kMinute = millisecond / 60;
        millisecond -= kMinute * 60;
        const long kSecond = millisecond;
        return str_format("%02ld:%02ld", kMinute, kSecond);
    }
    
    string TimeUtil::GetMMSSStrFromSecond(long spent_time)
    {
        return GetMMSSStrFromMillisecond(spent_time * 1000);
    }
    
    string TimeUtil::GetYYYYMMDDStr()
    {
        time_t time_now;
        time(&time_now);
        tm *t = localtime(&time_now);
        const int kYear = t->tm_year + 1900;
        const int kMonth = t->tm_mon + 1;
        const int kDay = t->tm_mday;
        return str_format("%04d-%02d-%02d", kYear, kMonth, kDay);
    }
    
    int TimeUtil::GetDayBySecond(long nSecond)
    {
        time_t time_now = nSecond;
        tm *t = localtime(&time_now);
        const int kDay = t->tm_mday;
        return kDay;
    }
    
    int TimeUtil::GetMonthBySecond(long nSecond)
    {
        time_t time_now = nSecond;
        tm *t = localtime(&time_now);
        const int kMonth = t->tm_mon + 1;
        return kMonth;
    }
    
    int TimeUtil::GetYearBySecond(long nSecond)
    {
        time_t time_now = nSecond;
        tm *t = localtime(&time_now);
        const int kYear = t->tm_year + 1900;
        return kYear;
    }
    
    int TimeUtil::GetWeekDayBySecond(long nSecond)
    {
        time_t time_now = nSecond;
        tm *t = localtime(&time_now);
        const int kWeekDay = t->tm_wday;
        return kWeekDay;
    }
    
    int TimeUtil::GetWeeksOfYear()
    {
        int nWeek = 0;
        time_t time_now;
        time(&time_now);
        const int nCurYear = GetYearBySecond(time_now);
        
        long nFirstSeconds = 0;
        long nDays = 0;
        for (int nYear = 1970; nYear < nCurYear; nYear++)
        {
            if ((nYear % 4 == 0 && nYear % 100 != 0) || nYear % 400 == 0)
            {
                nDays += 366;
            }
            else
            {
                nDays += 365;
            }
        }
        nFirstSeconds = nDays * 24 * 60 * 60;
        
        tm oTm = TimeUtil::GetDatetimeFromFormatString(str_format("%d-01-01 00:00:00", nCurYear));
        int nFirstDayOfWeek = oTm.tm_wday;                               // 当年第一天是周几
        long nIntervalDay = (time_now - nFirstSeconds) / 24 / 60 / 60;   // 今天与当年第一天数间隔
        nFirstDayOfWeek = nFirstDayOfWeek == 0 ? 7 : nFirstDayOfWeek;    // 第一天周天 0 转为 7
        nWeek = (int)(nIntervalDay + nFirstDayOfWeek - 1 + 7) / 7;       // 计算是第几周
        return nWeek;
    }
    
    // nYear(2015), nMonth(1-12)
    int TimeUtil::GetDaysByYearAndMonth(int nYear, int nMonth)
    {
        int nDays = 0;
        switch(nMonth)
        {
            case 4:
            case 6:
            case 9:
            case 11:
                nDays = 30;
                break;
            case 2:
                if ((nYear % 4 == 0 && nYear % 100 != 0) || nYear % 400 == 0)
                {
                    nDays = 29;
                }
                else
                {
                    nDays = 28;
                }
                break;
            default:
                nDays = 31;
                break;
        }
        return nDays;
    }
    string TimeUtil::ConvertToStr(int nTime)
    {
        time_t t = nTime;
        struct tm *p;
        p=gmtime(&t);
        char s[100];
        strftime(s, sizeof(s), "%Y-%m-%d %H:%M:%S", p);
        return s;
    }
    
    tm TimeUtil::GetDatetimeFromFormatString(const std::string &sDateTime)
    {
        tm oTm = {0};
        strptime(sDateTime.c_str(), "%Y-%m-%d %H:%M:%S", &oTm);
        int year = oTm.tm_year + 1900;
        int month = oTm.tm_mon + 1;
        int day = oTm.tm_mday;
        int m = month;
        if (month == 1)
        {
            m = 13;
            year -= 1;
        }
        else if (month == 2)
        {
            m = 14;
            year -= 1;
        }
        int y = year % 100;
        int c = year / 100;
        int d = day;
        int w = (y + (y / 4) + (c / 4) - (2 * c) + (26 * (m + 1) / 10) + (d - 1)) % 7;
        oTm.tm_wday = w;
        return oTm;
    }
    
    int TimeUtil::GetDaysBySecond(int nSecond)
    {
        int nDays = 0;
        if (nSecond < 0)
        {
            return nDays;
        }
        nDays = nSecond / 86400;
        if (nSecond % 86400 > 0)
        {
            nDays += 1;
        }
        return nDays;
    }
    
    tm TimeUtil::GetDatetimeStepDay(const std::string& old_day,const int step)
    {
        tm old_time = GetDatetimeFromFormatString(old_day);
                
        long seconds = 24 * 3600 * step;
        
        time_t old = mktime(&old_time);
        
        old += seconds;
        
        tm* now_day = gmtime(&old);
        
        return *now_day;
    }
    
    int TimeUtil::GetBeiJingTimeFromLocal()
    {
        time_t time_now = time(NULL);
        struct tm tm_Local;
        localtime_r(&time_now, &tm_Local);
        
        time_t time_beijing = time_now - tm_Local.tm_gmtoff + 28800;
        
        return (int)time_beijing;
    }
    
    tm TimeUtil::GetBeiJingTMFromLocal()
    {
        time_t time_now = time(NULL);
        struct tm tm_Local;
        localtime_r(&time_now, &tm_Local);
        
        time_t time_beijing = time_now - tm_Local.tm_gmtoff + 28800;
        struct tm tm_BeiJing;
        localtime_r(&time_beijing, &tm_BeiJing);
        
        return tm_BeiJing;
    }
    
    tm TimeUtil::GetBeiJingTMFromData(int nTime)
    {
        time_t time_data = nTime;
        struct tm tm_Local;
        localtime_r(&time_data, &tm_Local);
        
        time_data += (28800 - tm_Local.tm_gmtoff);
        struct tm tm_BeiJing;
        localtime_r(&time_data, &tm_BeiJing);
        
        return tm_BeiJing;
    }
    //将秒 转化为 天 时
    std::string TimeUtil::SecondsConvertoStrtimeGame(long lLeftTime)
    {
        std::string ret;
        int nDay=0, nHour=0, nMin=0;
        nDay = (int)(lLeftTime / 3600 / 24);
        lLeftTime %= (24 * 3600);
        nHour = (int)(lLeftTime / 3600);
        lLeftTime %= (3600);
        nMin = (int)(lLeftTime / 60);
        lLeftTime %= 60;
        
        char buffer[128]={0};
        if (nDay > 0 )
        {
            snprintf(buffer, 128, "%d天%d小时", nDay, nHour);
        }
        else if(nHour > 0)
        {
            if (nHour >= 1000) {
                snprintf(buffer, 128, "%d小时", nHour);
            }
            else
            {
                if (nMin != 0) {
                    snprintf(buffer, 128, "%d时%d分", nHour, nMin);
                }
                else
                {
                    snprintf(buffer, 128, "%d小时", nHour);
                }
            }
        }
        else
        {
            snprintf(buffer,128,"%d分", nMin);
        }
        
        ret = buffer;
        return ret;
    }
}
