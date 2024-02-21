//
//  mylibwebsocket.h
//  GuanDanConnSvrd
//
//  Created by huguangfei on 2018/5/22.
//  Copyright © 2018年 liuting. All rights reserved.
//

#ifndef mylibwebsocket_h
#define mylibwebsocket_h

#include <cstdlib>
#include <libwebsockets.h>
#include "mylibevent.h"

int lws_server_socket(int port);

bool update_writable(struct lws *wsi);

bool lws_period_established(struct lws *wsi, conn *, DWORD seqid);

bool lws_period_receive(conn *c, void *in, size_t len);

bool lws_period_writeable(struct lws *wsi, conn *c);

bool lws_period_closed(conn *c);

bool lws_period_pingpong(struct lws *wsi);

//断线
bool client_break(conn *c, int close_type);

bool del_ws_coon(DWORD seqid);

bool release_wsi_obj(conn *c);

//资源释放
void release_wsi_resource();

//打印运行状态
void dump_ws_statinfo();

//扫描断线状态
void scan_ws_client();

#endif /* mylibwebsocket_h */
