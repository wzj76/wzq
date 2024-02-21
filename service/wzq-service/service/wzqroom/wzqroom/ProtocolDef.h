#ifndef __PROTOCOL_DEF_H__
#define __PROTOCOL_DEF_H__

//	消息基础值定义
#define MSGID_BASE_SVRD_INSIDE  0x00000400      // 服务内部通信
#define	MSGID_BASE_GS_GAMEINFO	0x00000500		// gamesvrd和gameinfo交互
#define	MSGID_BASE_GS_WEB		0x00000600		// gamesvrd和web交互
#define	MSGID_BASE_GS_MANAGER	0x00000700		// gamesvrd和manager交互
#define MSGID_BASE_GS_AI		0x00000800		// gamesvrd和ai交互
#define MSGID_BASE_GS_RM        0x00000900      // 同roomsvrd 交互
#define	MSGID_BASE_GS_CLIENT	0x00010000		// gamesvrd和client交互

#endif


#define MSGID_MATCH                 (MSGID_BASE_GS_CLIENT + 3)      //匹配



#define MSGID_CREATE_GAME              (MSGID_BASE_GS_CLIENT + 10)      //创建房间

#define MSGID_FORCE_EXIT               (MSGID_BASE_GS_CLIENT + 20)      //用户强制对出游戏

#define MSGID_INVITE_FRIEND_GAME              (MSGID_BASE_GS_CLIENT + 55)      //玩家邀请好友继续游戏
#define MSGID_ASK_INVITE_FRIEND_GAME                (MSGID_BASE_GS_CLIENT + 56)      //询问好友是否同意
#define MSGID_REFUSE_INVITE_FRIEND_GAME                (MSGID_BASE_GS_CLIENT + 57)      //好友拒绝继续游戏
