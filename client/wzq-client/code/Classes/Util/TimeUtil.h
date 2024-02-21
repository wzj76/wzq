/*******************************************************************************
 计算时间的类。功能有：
 1. 计算时间差并表示出来:
 开始计时时调用Start，需要暂停计时时调用Pause，恢复计时时调用Resume，结束计时调用End。
 注意Pause和Resume必须成对出现。pair.first为Pause，pair.second为Resume
 举个例子，比如暂停恢复了两次：
 Start(t0)  Pause（t1)  Resume(t2)  Pause(t3)  Resume(t4) Pause(t5) CalculateSpentTime()
 那么真正的时间为总时间减去暂停时间，即：(t5 - t0) - ((t2 - t1) + (t4 - t3))。
 2. MillisecondToNow获取当前时间（cocos2d-x的系统时间，不能用来计算当前日期），毫秒表示。
 3. GetHHMMSSStr将花费的时间格式化成"hh:mm:ss"形式。
 4. GetMMSSStr将花费的时间格式化成"mm:ss"形式。
 5. GetYYYYMMDDStr获取当前时间，格式为YYYY-MM-DD。
 *******************************************************************************/

#pragma once

#include "cocos2d.h"

namespace util
{
    
    using std::pair;
    using std::string;
    
    class TimeUtil : public cocos2d::Ref
    {
    public:
        TimeUtil();
        
        // 开始计时
        void Start();
        
        // 暂停计时
        void Pause();
        
        // 恢复计时
        void Resume();
        
        // 重新开始
        void Restart();
        
        // 统计到当前位置花费的时间，返回游戏时间, 用pair存储
        // pair.first表示总时间长度，pair.second是字符串表示，比如10分钟23秒00:10:23
        std::pair<int, std::string> CalculateSpentTime();
        // 获取cocos2d-x系统时间（毫秒级）
        static long long MillisecondToNow();
        // 系统时间秒
        static long SecondToNow();
        
        // 根据时间（毫秒）生成对应的时间串，格式为 00:03:05，即0时3分5秒
        static string GetHHMMSSStrFromMillisecond(long spent_time);
        
        // 根据时间（秒）生成对应的时间串，格式为 00:03:05，即0时3分5秒
        static string GetHHMMSSStrFromSecond(long spent_time);
        
        // 根据时间（毫秒）生成对应的时间串，格式为 03:05，即3分5秒
        static string GetMMSSStrFromMillisecond(long spent_time);
        
        // 根据时间（毫秒）生成对应的时间串，格式为 03:05，即3分5秒
        static string GetMMSSStrFromSecond(long spent_time);
        
        // 获取当前时间， 格式为YYYY-MM-DD
        static string GetYYYYMMDDStr();
        //
        
        //转换时间戳为时间字符串  格式yyyy-mm-dd
        static string ConvertToStr(int nTime);
        
        static int GetYearBySecond(long nSecond);
        
        static int GetMonthBySecond(long nSecond);
        
        static int GetDayBySecond(long nSecond);
        
        static int GetWeekDayBySecond(long nSecond);
        
        static int GetWeeksOfYear();                            // 获取今天是今年的第几周
        
        // nYear(2015), nMonth(1-12)
        static int GetDaysByYearAndMonth(int nYear, int nMonth);
        // 获取时间字符串（格式必须为"2016-03-30 17:33:25"）对应的时间
        static tm GetDatetimeFromFormatString(const std::string& sDateTime);
        static int GetDaysBySecond(int nSecond);
        //获取old_day的前或者后step天
        static tm GetDatetimeStepDay(const std::string& old_day,const int step);

        //以北京事件为准
        static int GetBeiJingTimeFromLocal();
        static tm GetBeiJingTMFromLocal();
        static tm GetBeiJingTMFromData(int nTime);
        
        static std::string SecondsConvertoStrtimeGame(long lLeftTime);
        
    private:
        int begin_time_;                                       // 开始时间
        std::vector<std::pair<int, int> > pause_resume_note_;  // 暂停、恢复记录。first为暂停时间，second为恢复时间
    };
}
