//
//  InviteFriendGame.cpp
//  wzqroom
//
//  Created by duole on 2024/2/18.
//

#include "InviteFriendGame.hpp"
#include "matcher.hpp"
void InviteFriendGame::InviteGame(int user_id,PlayerInfo&info,int friend_id)
{
    m_umPlayerInfoDeque[user_id] = info;
    m_umInvite[user_id] = friend_id;
    
    if(m_umInvite.find(friend_id)!=m_umInvite.end()&&m_umInvite[friend_id]==user_id)
    {
        //friend也同时发起了邀请请求，直接给他们创建游戏
        //检查这两个人的信息是否存在
        if(m_umPlayerInfoDeque.find(friend_id)!=m_umPlayerInfoDeque.end()&&m_umPlayerInfoDeque.find(user_id)!=m_umPlayerInfoDeque.end())
        {
            printf("两个人同时发起了邀请请求，邀请成功，id1:%d,id2:%d\n",user_id,friend_id);
            //找到两个人的信息，进行游戏
            PlayerInfo playerInfo1 = m_umPlayerInfoDeque[friend_id];
            PlayerInfo playerInfo2 = m_umPlayerInfoDeque[user_id];
            //随机选择一个人成为黑棋
            int black_id = 0;
            int white_id = 0;
            if(std::rand()%2 ==0)
            {
                black_id = playerInfo1.m_userid;
                white_id = playerInfo2.m_userid;
            }
            else
            {
                black_id = playerInfo2.m_userid;
                white_id = playerInfo1.m_userid;
            }
            Matcher* p_matcher = m_room_service ->m_matcher;
            
            p_matcher->SendMatchMessage(playerInfo1,playerInfo2,black_id,white_id);
            p_matcher->SendMatchMessage(playerInfo2,playerInfo1,black_id,white_id);
            
            //需要向game服务器发送消息创建房间
            p_matcher->SendCreateGameMessage(playerInfo1,playerInfo2,black_id,white_id);
            m_umInvite.erase(friend_id);
            m_umInvite.erase(user_id);
            m_umPlayerInfoDeque.erase(friend_id);
            m_umPlayerInfoDeque.erase(user_id);
        }
       
        return;
    }
    printf("发送ACKInviteFriendGame\n");
    ACKInviteFriendGame ack;
    ack.set_code(0);
    ack.set_result_msg("等待好友同意游戏");
    m_room_service->SendSvrdMsg(m_room_service->GetConnSvrdMsgRoute(), (MSGID_INVITE_FRIEND_GAME | ID_ACK), 0, 0, ack.SerializeAsString(), info.m_clientid);

    printf("发送REQAskInviteFriendGame\n");
    //询问好友是否同意继续游戏
    REQAskInviteFriendGame req;
    req.mutable_friend_info()->CopyFrom(info.m_userinfo);
    req.set_user_id(friend_id);
    m_room_service->SendSvrdMsg(m_room_service->GetConnSvrdMsgRoute(), (MSGID_ASK_INVITE_FRIEND_GAME | ID_REQ), 0, 0, req.SerializeAsString(), 0);
    
//    //询问对手是否同意继续游戏
//    REQAskContinueGame req_ask;
//    req_ask.mutable_enemyinfo()->CopyFrom(req.userinfo());
//    req_ask.set_user_id(enemy_id);
//    SendSvrdMsg(GetConnSvrdMsgRoute(), (MSGID_ASK_CONTINUE_GAME | ID_REQ), 0, 0, req_ask.SerializeAsString(), 0);
}

void InviteFriendGame::IsAgreeInviteGame(int user_id,PlayerInfo&info,int friend_id,bool is_agree,std::string&msg)
{
    if(m_umPlayerInfoDeque.find(friend_id)==m_umPlayerInfoDeque.end())
    {
        printf("error... 找不到friend_id:%\n",friend_id);
        return;
    }
    
    
    if(is_agree == true&&m_umPlayerInfoDeque.find(friend_id)!=m_umPlayerInfoDeque.end())
    {
        printf("同意继续游戏，开始创建游戏\n");
        PlayerInfo playerInfo1 = m_umPlayerInfoDeque[friend_id];
        PlayerInfo playerInfo2 = info;
        //随机选择一个人成为黑棋
        int black_id = 0;
        int white_id = 0;
        if(std::rand()%2 ==0)
        {
            black_id = playerInfo1.m_userid;
            white_id = playerInfo2.m_userid;
        }
        else
        {
            black_id = playerInfo2.m_userid;
            white_id = playerInfo1.m_userid;
        }
        Matcher* p_matcher = m_room_service ->m_matcher;
        
        p_matcher->SendMatchMessage(playerInfo1,playerInfo2,black_id,white_id);
        p_matcher->SendMatchMessage(playerInfo2,playerInfo1,black_id,white_id);
        
        //需要向game服务器发送消息创建房间
        p_matcher->SendCreateGameMessage(playerInfo1,playerInfo2,black_id,white_id);
        
        m_umInvite.erase(friend_id);
        m_umInvite.erase(user_id);
        m_umPlayerInfoDeque.erase(friend_id);
        m_umPlayerInfoDeque.erase(user_id);
    }
    else if(is_agree == false)
    {
        PlayerInfo playerInfo1 = m_umPlayerInfoDeque[friend_id];
        ACKRefuseInviteFriendGame ack;
        ack.set_code(1);
        ack.set_result_msg(msg);
        m_room_service->SendSvrdMsg(m_room_service->GetConnSvrdMsgRoute(), (MSGID_REFUSE_INVITE_FRIEND_GAME | ID_ACK), 0, 0, ack.SerializeAsString(), playerInfo1.m_clientid);
        m_umInvite.erase(friend_id);
        m_umInvite.erase(user_id);
        m_umPlayerInfoDeque.erase(friend_id);
        m_umPlayerInfoDeque.erase(user_id);
    }
}



InviteFriendGame::InviteFriendGame()
{}

InviteFriendGame::InviteFriendGame(CRoomService* room_service)
    :m_room_service(room_service)
    {}
InviteFriendGame::~InviteFriendGame(){}
