#ifndef __PROTOCOL_DEF_H__
#define __PROTOCOL_DEF_H__

#include "paohuziProtocol.pb.h"

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
#define	MSGID_BASE_GS_GAMEINFO	0x00000500		// gamesvrd和gameinfo交互
#define	MSGID_BASE_GS_WEB		0x00000600		// gamesvrd和web交互
#define	MSGID_BASE_GS_MANAGER	0x00000700		// gamesvrd和manager交互
#define MSGID_BASE_GS_AI		0x00000800		// gamesvrd和ai交互
#define MSGID_BASE_GS_RM        0x00000900      // roomsvrd和game交互
#define	MSGID_BASE_GS_CLIENT	0x00010000		// gamesvrd和client交互
#define MSGID_BASE_RS_CLIENT    0x00020000      // roomsvrd和client交互
#define MSGID_BASE_COMMON       0x00030000      // 通用的信息ID
#define MSGID_BASE_BS_CLIENT    0X00040000      // business和client交互
#define MSGID_BASE_GS_ATTACH    0x00050000      // 附属游戏(与其他游戏有区别，与我们的房间协议冲突了)
//------------------------------------登录---------------------------------------
// 注册
#define  MSGID_REGISTER				(MSGID_BASE_GS_CLIENT + 1)
class REQUserRegister;
class ACKUserRegister;
// 登录
#define  MSGID_LOGIN                (MSGID_BASE_GS_CLIENT + 2)


//匹配
#define MSGID_MATCH                 (MSGID_BASE_GS_CLIENT + 3)

//编辑信息
#define MSGID_EDIT                  (MSGID_BASE_GS_CLIENT + 4)

//排行榜
#define RANDING_LIST                  (MSGID_BASE_GS_CLIENT + 5)
//用户信息
#define MSGID_GET_USER_INFO                  (MSGID_BASE_GS_CLIENT + 6)

#define MSGID_CREATE_GAME              (MSGID_BASE_GS_CLIENT + 10)      //创建房间
#define MSGID_ACTION                   (MSGID_BASE_GS_CLIENT + 11)      //用户下棋或者悔棋的操作
#define MSGID_GMAE_OVER          (MSGID_BASE_GS_CLIENT + 12)      //游戏结束，发给auth用来给用户加分和减分
#define MSGID_EXIT_GAME                  (MSGID_BASE_GS_CLIENT + 13)      //用户退出游戏
#define MSGID_ASK_UNDO_CHESS                 (MSGID_BASE_GS_CLIENT + 14)      //询问玩家是否同意对手悔棋

#define MSGID_GET_HISTORY          (MSGID_BASE_GS_CLIENT + 30)      //获取用户的历史记录
#define MSGID_EXIT_ACCOUNT         (MSGID_BASE_GS_CLIENT + 31)      //用户注销账号
#define MSGID_LOGIN_DIFFERENT         (MSGID_BASE_GS_CLIENT + 32)      //用户异地登录

#define MSGID_CHAT         (MSGID_BASE_GS_CLIENT + 33)      //聊天

#define MSGID_GET_FRIEND_INFO         (MSGID_BASE_GS_CLIENT + 40)      //查找用户信息，来添加好友
#define MSGID_ADD_FRIEND       (MSGID_BASE_GS_CLIENT + 41)   //添加好友  和  同意或拒绝添加好友
#define MSGID_REQ_FRIEND       (MSGID_BASE_GS_CLIENT + 42)   //提醒用户有n条好友请求
#define MSGID_GET_FRIENDS_LIST       (MSGID_BASE_GS_CLIENT + 43)   //查找好友列表
#define MSGID_GET_WAIT_FRIEND_LIST      (MSGID_BASE_GS_CLIENT + 44)   //查找需要同意的好友列表
#define MSGID_IS_AGREE_ADD_FRIEND      (MSGID_BASE_GS_CLIENT + 45)   //同意或不同意添加好友的消息
#define MSGID_DELETE_FRIEND      (MSGID_BASE_GS_CLIENT + 46)   //删除好友


#define MSGID_CONTINUE_GAME               (MSGID_BASE_GS_CLIENT + 50)      //玩家和对手继续游戏
#define MSGID_ASK_CONTINUE_GAME               (MSGID_BASE_GS_CLIENT + 51)   //询问对手是否同意继续游戏
#define MSGID_REFUSE_GAME               (MSGID_BASE_GS_CLIENT + 52)      //对方拒绝继续游戏

#define MSGID_INVITE_FRIEND_GAME              (MSGID_BASE_GS_CLIENT + 55)      //玩家邀请好友继续游戏
#define MSGID_ASK_INVITE_FRIEND_GAME                (MSGID_BASE_GS_CLIENT + 56)      //询问好友是否同意
#define MSGID_REFUSE_INVITE_FRIEND_GAME                (MSGID_BASE_GS_CLIENT + 57)      //好友拒绝继续游戏
#define MSGID_BUY                              (MSGID_BASE_GS_CLIENT + 58)  //购买商品
//修改昵称
#define MSGID_EDIT_NAME                  (MSGID_BASE_GS_CLIENT + 59)

class REQUserLogin;
class ACKUserLogin;

#endif
