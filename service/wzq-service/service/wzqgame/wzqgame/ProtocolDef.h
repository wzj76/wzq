#ifndef __PROTOCOL_DEF_H__
#define __PROTOCOL_DEF_H__

#include "gameProtocol.pb.h"
#include "protocol.h"
//---------------------------------------------------------------------------
//	操作类型定义 共9种
//	28位至21位，共8位一个字节，说明消息操作类型，最多可以表示9种类型
//	0000 1111 1111 0000 0000 0000 0000 0000
//---------------------------------------------------------------------------
#define MSGID_REQ					0x00000000		//请求消息类型
#define MSGID_ACK					0x02000000		//应答消息类型
#define MSGID_NTF					0x04000000		//通知消息

//---------------------------------------------------------------------------
//	标准网络通信头字段使用定义
//---------------------------------------------------------------------------
//	dwType 32位4字节格式使用定义
//---------------------------------------------------------------------------
//	版本号定义1~16
//	32位至29位，共4位半个字节，说明消息版本号，从0开始共可表示16个版本
//	1111 0000 0000 0000 0000 0000 0000 0000
//---------------------------------------------------------------------------

//	消息基础值定义
#define MSGID_BASE_SVRD_INSIDE  0x00000400      // 服务内部通信
#define	MSGID_BASE_GS_GAMEINFO	0x00000500		// gamesvrd和gameinfo交互
#define	MSGID_BASE_GS_WEB		0x00000600		// gamesvrd和web交互
#define	MSGID_BASE_GS_MANAGER	0x00000700		// gamesvrd和manager交互
#define MSGID_BASE_GS_AI		0x00000800		// gamesvrd和ai交互
#define MSGID_BASE_GS_RM        0x00000900      // 同roomsvrd 交互
#define	MSGID_BASE_GS_CLIENT	0x00010000		// gamesvrd和client交互

#endif



#define MSGID_CREATE_GAME              (MSGID_BASE_GS_CLIENT + 10)      //创建房间

#define MSGID_ACTION           (MSGID_BASE_GS_CLIENT + 11)      //用户下棋或者悔棋的操作

#define MSGID_GMAE_OVER          (MSGID_BASE_GS_CLIENT + 12)      //游戏结束，发给auth用来给用户加分和减分

#define MSGID_EXIT_GAME                  (MSGID_BASE_GS_CLIENT + 13)      //用户退出游戏

#define MSGID_ASK_UNDO_CHESS                 (MSGID_BASE_GS_CLIENT + 14)      //询问玩家是否同意对手悔棋

#define MSGID_FORCE_EXIT               (MSGID_BASE_GS_CLIENT + 20)      //用户强制对出游戏

#define MSGID_CHAT         (MSGID_BASE_GS_CLIENT + 33)      //聊天

#define MSGID_CONTINUE_GAME               (MSGID_BASE_GS_CLIENT + 50)      //玩家和对手继续游戏
#define MSGID_ASK_CONTINUE_GAME               (MSGID_BASE_GS_CLIENT + 51)      //询问对手是否同意继续游戏
#define MSGID_REFUSE_GAME               (MSGID_BASE_GS_CLIENT + 52)      //对方拒绝继续游戏
