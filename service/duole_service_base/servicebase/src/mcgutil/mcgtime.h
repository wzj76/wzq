//
//  mcgtime.h
//  servicebase
//
//  Created by 曹涛 on 2017/8/22.
//  Copyright © 2017年 曹涛. All rights reserved.
//

#ifndef mcgtime_h
#define mcgtime_h

#include <stdio.h>
#include <time.h>
#include <string>

//时间工具类

std::string GetCurrentDayTime();
//获取当前日期 精确到秒
std::string GetCurrentDateTime();
//获取当前日期 精确到天
std::string GetCurrentDay();
//获取当日的零点
std::string GetCurrentDayZero();

std::string ConvertToStr(tm t);

//获取本周一的日期
tm* GetMondayDate();
//获取当前日期 精确到秒
std::string GetCurrentTime();
//获取当前时间
tm* GetNowDate();

//得到当前毫秒数
long long currUSec();
time_t MakeTime(const char *szDateTime);

//格式化到零点
std::string FormatDateZero(const tm &t);
std::string FormatDateZero(const std::string &sDateTime);

//
tm GetDatetimeFromFormatString(const std::string &sDateTime);

tm GetDatetimeStepDay(const std::string& old_day,const int step);

//是否在当前时间之后
bool IsAfterWithNow(const std::string& sDateTime);
//是否在当天之后 单位是天
bool IsAfterWithToday(const std::string& sDateTime);

//是否在当前时间之前
bool IsBeforeWithNow(const std::string& sDateTime);
//是否在当天之前 单位是天
bool IsBeforeWithToday(const std::string& sDateTime);

//格式化输出当前时间
std::string strfcurrtime(const std::string& format);
//格式化输出当前时间
std::string strfcurrtimev2(const std::string& format , time_t timestamp);

#endif /* mcgtime_h */
