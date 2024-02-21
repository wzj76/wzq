#ifndef __MY_LIBEVENT_H__
#define __MY_LIBEVENT_H__

#include <assert.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <event.h>

#include "mcgclient.h"
#include "datatype.h"


#define DATA_BUFFER_SIZE		2048
#define UDP_READ_BUFFER_SIZE	65536
#define TCP_READ_BUFFER_SIZE	65536
#define UDP_MAX_PAYLOAD_SIZE	1400
#define UDP_HEADER_SIZE			8
#define MAX_SENDBUF_SIZE		(256 * 1024 * 1024)
#define MAX_CONN				65535
#define START_SEQID				1
#define BACK_LOG				256
#define UNSIGNED_INT_MAX		0x000FFFFF
#define HEADR_SIZE			sizeof(HEADER)
#define SEQ_START_ADDR			2*sizeof(DWORD)
#define LEN_START_ADDR			sizeof(DWORD)
#define MAX_PACKET				64*1024
#define TIMEOUT_1S				1*1000	
#define TIMEOUT_100MS			100		
#define TIMEOUT_10MS			10		
#define TIMEOUT_0MS				0

#define ONBREAK_CLOSE		1	//
#define ONBREAK_TIMEOUT		2	//
#define ONBREAK_SOCKERR		3	//

#define ONBREAK_CONN        0
#define ONBREAK_CLOSE        1    //
#define ONBREAK_TIMEOUT        2    //
#define ONBREAK_SOCKERR        3    //
#define ONBREAK_PLAYER      4
#define ONBREAK_SHUTDOWN    5

// A connection type
enum conn_type 
{
	conn_listen=1,	// listen connection
	conn_rw=2,		// read or write connection
	conn_mount=3,	// mount read
    conn_msg_queue = 4 ,
    conn_lws_pro   = 10 ,    //libwebsocket protocol
	//conn_max_state	// max conn type value (used for assertion)
};

typedef struct conn conn;
struct conn 
{
	int sfd;
	enum conn_type type;
	struct event event;
	short ev_flags;
	short  which;   /** which events were just triggered */
	CMCGClient *client_agent;
	int listen_port;
	long long start_read;
	long long start_write;
	unsigned long long queue_size;//
};

typedef struct sockpair sockpair;
struct sockpair 
{
	int notify_recv_sfd;
	int notify_send_sfd;
	conn *c;
};

typedef struct statinfo statinfo;
struct statinfo 
{
	unsigned long long accept_failed;	// 接收连接失败数
	unsigned long long current_conn;	// 当前连接数
	unsigned long long active_close;	// 主动断开连接数
	unsigned long long passive_close;	// 被动断开连接数
	unsigned long long sockerr_close;	// SOCKET异常断开连接数
	unsigned long long eventerr_opt;	// libevent操作异常数
};

//处理消息使用时间
typedef struct _MSG_TIME{
    unsigned long long usetime3ms;          //处理消息耗时0-3毫秒
    unsigned long long usetime5ms;         //处理消息耗时3-5毫秒
    unsigned long long usetime10ms;         //处理消息耗时5-10毫秒
    unsigned long long usetime20ms;         //处理消息耗时10-20毫秒
    unsigned long long usetime50ms;        //处理消息耗时20-50毫秒
    unsigned long long usetime100ms;        //处理消息耗时50-100毫秒
    unsigned long long usetime999ms;        //处理消息耗时大于100毫秒
    _MSG_TIME():
    usetime3ms(0),
    usetime5ms(0),
    usetime10ms(0),
    usetime20ms(0),
    usetime50ms(0),
    usetime100ms(0),
    usetime999ms(0){}
}MSG_TIME;

conn *conn_new(const int sfd, enum conn_type init_type, const int event_flags, struct event_base *base, 
			   unsigned long seqid=0, DWORD dwIP=0, WORD wPort=0);
int server_socket(int port);
void event_handler(const int fd, const short which, void *arg); 
bool update_event(conn *c, const int new_flags);
void conn_close(conn *c);
void release_resource();
void dump_statinfo();
void time_cb(int fd, short event, void *argc);
void app_time_cb(void* pContext, DWORD dwEventID);
void scan_client();
bool conn_obj_exist(DWORD seqid);
CMCGClient *get_conn_obj(DWORD seqid);
DWORD current_conn_obj();
void scan_halfstatus_client();
//统计各消息处理时间
void CalOnMsgDealTime(DWORD dwMsgID, long long use_time);
void SendOnMsgTimeLog();

#endif // __MY_LIBEVENT_H__
