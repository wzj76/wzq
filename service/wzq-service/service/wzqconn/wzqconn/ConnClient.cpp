#include "ConnClient.h"
#include "ConnService.h"
#include "log.h"
#include <iconv.h>
#include <errno.h>
#include <arpa/inet.h>
#include "wzqProtocol.pb.h"

#include "Tools.h"

extern CService *g_pService;

CConnClient::CConnClient(DWORD dwRemoteIP,WORD wListenPort):CMCGClient(dwRemoteIP,wListenPort)
{
    m_pService = (CConnService*)g_pService;
	m_pPlayer = NULL;
    m_bLogined = false ;
}

CConnClient::~CConnClient()
{
}

bool CConnClient::OnInit()
{
	m_pService=(CConnService *)g_pService;
	if (!CMCGClient::OnInit()) return false;
	return true;
}


bool CConnClient::OnMsg(DWORD dwMsgID, DWORD dwSeqID)
{
	if (CMCGClient::OnMsg(dwMsgID, dwSeqID)) return true;
    dwSeqID = 0 ;
    string strBuffer;
	McgPopPacket(strBuffer);
    
    if(dwMsgID == (MSGID_EDIT_NAME | ID_REQ))
    {
        printf("debug----------- conn收到一条修改名字的消息，转发给auth\n");
        m_pService ->SendSvrdMsg(m_pService->GetAuthSvrdMsgRoute(), dwMsgID, 0, 0, strBuffer, GetClientID());
        return true;
    }
    
    if(dwMsgID == (MSGID_BUY | ID_REQ))
    {
        printf("debug----------- conn收到一条购买商品的消息，转发给auth\n");
        m_pService ->SendSvrdMsg(m_pService->GetAuthSvrdMsgRoute(), dwMsgID, 0, 0, strBuffer, GetClientID());
        return true;
    }
    
    
    if(dwMsgID == (MSGID_ASK_INVITE_FRIEND_GAME | ID_ACK))
    {
        printf("debug----------- conn收到一条客户端发来的同意或拒绝的消息，转发给room\n");
        m_pService ->SendSvrdMsg(m_pService->GetRoomSvrdMsgRoute(), dwMsgID, 0, 0, strBuffer, GetClientID());
        return true;
    }
    
    
    
    if(dwMsgID == (MSGID_INVITE_FRIEND_GAME | ID_REQ))
    {
        REQInviteFriendGame req;
        req.ParsePartialFromString(strBuffer);
        CPlayer*  p_player = m_pService->m_pPlayerMgr -> GetPlayer(req.friend_id());
        if(p_player == nullptr)
        {
            //好友不在线，直接给它发送ack
            ACKInviteFriendGame ack;
            ack.set_code(1);
            ack.set_result_msg("好友不在线");
            m_pService->SendClientMsg(0, MSGID_INVITE_FRIEND_GAME | ID_ACK, ack.SerializeAsString(), 0,GetClientID());
            return true;
        }
        
        printf("debug----------- conn收到一条邀请好友继续游戏的消息，转发给room\n");
        m_pService ->SendSvrdMsg(m_pService->GetRoomSvrdMsgRoute(), dwMsgID, 0, 0, strBuffer, GetClientID());
        return true;
    }
    
    
    
    
    if(dwMsgID == (MSGID_ASK_CONTINUE_GAME | ID_ACK))
    {
        printf("debug----------- conn收到一条同意或拒绝继续游戏的消息，转发给game\n");
        m_pService ->SendSvrdMsg(m_pService->GetGameSvrdMsgRoute(), dwMsgID, 0, 0, strBuffer, GetClientID());
        return true;
    }
    
    
    if(dwMsgID == (MSGID_CONTINUE_GAME | ID_REQ))
    {
        printf("debug----------- conn收到一条继续游戏的消息，转发给game\n");
        m_pService ->SendSvrdMsg(m_pService->GetGameSvrdMsgRoute(), dwMsgID, 0, 0, strBuffer, GetClientID());
        return true;
    }
    
    //删除好友
    if(dwMsgID == (MSGID_DELETE_FRIEND | ID_REQ))
    {
        printf("debug----------- conn收到一条删除好友的消息，转发给auth\n");
        m_pService ->SendSvrdMsg(m_pService->GetAuthSvrdMsgRoute(), dwMsgID, 0, 0, strBuffer, GetClientID());
        return true;
    }
    
    
    if(dwMsgID == (MSGID_GET_WAIT_FRIEND_LIST | ID_REQ))
    {
        printf("debug----------- conn收到一条获取wait好友列表的消息，转发给auth\n");
        m_pService ->SendSvrdMsg(m_pService->GetAuthSvrdMsgRoute(), dwMsgID, 0, 0, strBuffer, GetClientID());
        return true;
    }
    
    
    if(dwMsgID == (MSGID_GET_FRIENDS_LIST | ID_REQ))
    {
        printf("debug----------- conn收到一条获取好友列表的消息，转发给auth\n");
        m_pService ->SendSvrdMsg(m_pService->GetAuthSvrdMsgRoute(), dwMsgID, 0, 0, strBuffer, GetClientID());
        return true;
    }
    
    
    if(dwMsgID == (MSGID_IS_AGREE_ADD_FRIEND | ID_REQ))
    {
        printf("debug----------- conn收到一条同意或拒绝添加友的请求，转发给auth服务\n");
        m_pService ->SendSvrdMsg(m_pService->GetAuthSvrdMsgRoute(), dwMsgID, 0, 0, strBuffer, GetClientID());
        return true;
    }
    
    
    if(dwMsgID == (MSGID_ADD_FRIEND | ID_REQ))
    {
        printf("debug----------- conn收到一条添加友的请求，转发给auth服务\n");
        m_pService ->SendSvrdMsg(m_pService->GetAuthSvrdMsgRoute(), dwMsgID, 0, 0, strBuffer, GetClientID());
        return true;
    }
    
    
    if(dwMsgID == (MSGID_GET_FRIEND_INFO | ID_REQ))
    {
        printf("debug----------- conn收到一条查找好友的请求，转发给auth服务\n");
        m_pService ->SendSvrdMsg(m_pService->GetAuthSvrdMsgRoute(), dwMsgID, 0, 0, strBuffer, GetClientID());
        return true;
    }
    
    
    
    if(dwMsgID == (MSGID_CHAT | ID_REQ))
    {
        printf("debug----------- conn收到一条聊天的消息，转发给game服务\n");
        m_pService ->SendSvrdMsg(m_pService->GetGameSvrdMsgRoute(), dwMsgID, 0, 0, strBuffer, GetClientID());
        return true;
    }

    
    
    if(dwMsgID == (MSGID_GET_HISTORY | ID_REQ))
    {
        printf("debug----------- conn收到一条要获取历史记录的消息\n");
        m_pService ->SendSvrdMsg(m_pService->GetAuthSvrdMsgRoute(), dwMsgID, 0, 0, strBuffer, GetClientID());
        return true;
    }
    
    
    if(dwMsgID == (MSGID_GET_USER_INFO | ID_REQ))
    {
        //获取用户信息
        printf("debug----------- conn收到一条要发往auth获取用户信息的消息\n");
        m_pService ->SendSvrdMsg(m_pService->GetAuthSvrdMsgRoute(), dwMsgID, 0, 0, strBuffer, GetClientID());
        return true;
        
    }
    
    //注册消息
    if(dwMsgID == (MSGID_REGISTER | ID_REQ))
    {
        
        m_pService ->SendSvrdMsg(m_pService->GetAuthSvrdMsgRoute(), dwMsgID, 0, 0, strBuffer, GetClientID());
        return true;
    }
    
    //匹配消息
    if(dwMsgID == (MSGID_MATCH | ID_REQ))
    {
        printf("debug----------- conn收到一条要发往room的消息\n");
        m_pService ->SendSvrdMsg(m_pService->GetRoomSvrdMsgRoute(), dwMsgID, 0, 0, strBuffer, GetClientID());
        return true;
    }
    
    //编辑信息
    if(dwMsgID == (MSGID_EDIT_HEAD | ID_REQ)){
        printf("测试：数据库收到要修改的头像\n");
        m_pService ->SendSvrdMsg(m_pService->GetAuthSvrdMsgRoute(), dwMsgID, 0, 0, strBuffer, GetClientID());
        return true;
    }
    
    //排行榜消息
    if(dwMsgID == (RANDING_LIST | ID_REQ))
    {
        printf("debug----------- conn收到一条要获取排行榜的消息\n");
        m_pService ->SendSvrdMsg(m_pService->GetAuthSvrdMsgRoute(), dwMsgID, 0, 0, strBuffer, GetClientID());
        return true;
    }
    
    //下棋消息
    if(dwMsgID == (MSGID_ACTION | ID_REQ))
    {
        printf("debug----------- conn收到一条要下棋的消息\n");
        m_pService ->SendSvrdMsg(m_pService->GetGameSvrdMsgRoute(), dwMsgID, 0, 0, strBuffer, GetClientID());
        return true;
    }
    
    //用户退出消息
    if(dwMsgID == (MSGID_EXIT_GAME | ID_REQ))
    {
        printf("debug----------- conn收到一条要退出游戏的消息\n");
        
        //判断是退出游戏还是退出大厅
        REQExitGame req ;
        req.ParsePartialFromString(strBuffer);

        
        m_pService ->SendSvrdMsg(m_pService->GetGameSvrdMsgRoute(), dwMsgID, 0, 0, strBuffer, GetClientID());
        
    
        return true;
    }
    
    //用户对悔棋消息的应答
    if(dwMsgID == (MSGID_ASK_UNDO_CHESS | ID_ACK))
    {
        printf("debug:conn 收到一条用户悔棋的回应消息，转发给game\n");
        m_pService ->SendSvrdMsg(m_pService->GetGameSvrdMsgRoute(), dwMsgID, 0, 0, strBuffer, GetClientID());
        return true;
    }
    
    //用户注销
    if(dwMsgID == (MSGID_EXIT_ACCOUNT | ID_REQ))
    {
        printf("debug:conn 收到一条注销消息\n");
        if(m_bLogined == true)
        {
            int player_id = m_pPlayer -> m_nUserID;
            m_bLogined = false;
            m_pPlayer = nullptr;
            m_pService -> m_pPlayerMgr -> RemovePlayer(player_id);
        }
        return true;
    }
    
    
    
    if(! m_bLogined)
    {
        //这里只处理鉴权信息
        //请求登录的消息
        if(dwMsgID == (MSGID_LOGIN | ID_REQ))
        {
            m_pService ->SendSvrdMsg(m_pService->GetAuthSvrdMsgRoute(), dwMsgID, 0, 0, strBuffer, GetClientID());
            return true;
        }
        return true ;
    }
    if(m_pPlayer)
    {
        m_pPlayer ->OnMsg(dwMsgID, dwSeqID, (const byte*)strBuffer.data(), (int)strBuffer.length());
    }
    return true;
}

bool CConnClient::OnBreak(DWORD dwType)
{
    printf("debug::::调用OnBreak:client_id:%d,type:%d\n",GetClientID(),dwType);
    if(m_pPlayer)
    {
        int player_id = m_pPlayer -> m_nUserID;
        m_pPlayer ->Disconnect();
        m_pPlayer = nullptr;
        m_bLogined = false;
        m_pService -> m_pPlayerMgr -> RemovePlayer(player_id);
    }
    return true ;
}

bool CConnClient::OnTimeOut(DWORD dwEventID)
{
	if (CMCGClient::OnTimeOut(dwEventID)) 
	{
        int player_id = m_pPlayer -> m_nUserID;
        printf("超时....%d\n",player_id);
		return true;
	}
	return true;
}

