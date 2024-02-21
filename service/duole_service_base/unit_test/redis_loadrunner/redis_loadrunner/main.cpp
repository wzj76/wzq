//
//  main.cpp
//  redis_loadrunner
//
//  Created by 张建民 on 2021/8/18.
//

#include <iostream>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "mcgutil/RedisIntf.h"

using namespace std;

struct redis_conf {
    char ip[32];
    int port;
    char pwd[32];
};

redis_conf conf;

// 时间戳：毫秒；
long long getTimestampByMS()
{
    struct timeval current;
    gettimeofday(&current, NULL);
    long long currentTime = current.tv_sec ;
    currentTime = currentTime * 1000 + current.tv_usec/1000 ;
    return currentTime;
}

void redis_que_push(int nMax) {
    CRedisIntf* _pRedis = new CRedisIntf("loadrunner");
    if (!_pRedis) {
        return;
    }
    if (!_pRedis->init(conf.ip, conf.port, conf.pwd, 0))
    {
        printf("Error, redis_conect_test::, _pRedis init failed \n");
        return;
    }
    
    char szMsg[32] = {0};
    long long lltime = getTimestampByMS();
    for (int i = 0; i < nMax ; i++) {
        sprintf(szMsg, "list_msg_%d", i);
        _pRedis->list_rpush("redis_test", szMsg );
    }
    long long llDelta = getTimestampByMS() - lltime;
    float f = (float)llDelta/(float)nMax;
    printf("rpush : [%d  %lld %.4f]\n", nMax, llDelta, f);
}

/// 支持超时的redis对象：业务 redis 写
void redis_que_pop(int nMax){
    int _tt_connect = 10;
    int _tt_rw = 60;
    CRedisIntf* _pRedis_ttw = new CRedisIntf("loadrunner");
    if (!_pRedis_ttw) {
        return;
    }
    if (!_pRedis_ttw->init(conf.ip, conf.port, conf.pwd, _tt_connect, _tt_rw, 0))
    {
        printf("Error, redis_conect_test_ttw::, _pRedis_ttw init failed \n");
    }
    
    int i = 0;
    string sValue;
    long long lltime = getTimestampByMS();
    while (i < nMax) {
        _pRedis_ttw->list_blpop("redis_test", sValue , 60);
        i++;
    }
    long long llDelta = getTimestampByMS() - lltime;
    float f = (float)llDelta/(float)nMax;
    printf("blpop : [%d  %lld %.4f]\n", nMax, llDelta, f);
}

int main(int argc, const char * argv[]) {
    memset(&conf, 0 ,sizeof(conf));
    strcpy(conf.ip, "127.0.0.1");
    conf.port = 6379;
    strcpy(conf.pwd, "");

    std::cout << "Ready Go ... \n";
    char cmd;
    int nMax = 0;
    do {
        std::cout << "\nPlease Enter Command: c-rpush  s-blpop  q-quit i-redis config " << std::endl;
        std::cin >> cmd;
        if (cmd == 'c') {
            std::cout << "Input rpush Count: " << std::endl;
            std::cin >> nMax;
            redis_que_push(nMax);
        } else if (cmd == 's') {
            std::cout << "Input blpop Count: " << std::endl;
            std::cin >> nMax;
            redis_que_pop(nMax);
        } else if (cmd == 'i') {
            std::cout << "Please Enter Redis Conf[ip port pwd]: " << std::endl;
            std::cin >> conf.ip >> conf.port >> conf.pwd;
        }
    
    } while (cmd != 'q');
    return 0;
}
