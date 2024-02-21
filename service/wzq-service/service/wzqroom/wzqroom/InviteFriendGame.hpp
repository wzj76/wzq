//
//  InviteFriendGame.hpp
//  wzqroom
//
//  Created by duole on 2024/2/18.
//

#ifndef InviteFriendGame_hpp
#define InviteFriendGame_hpp

#include <stdio.h>
#include <stdio.h>
#include <unordered_map>
#include "PlayerInfo.hpp"
class CRoomService;
class InviteFriendGame
{
public:
    
    InviteFriendGame();
    InviteFriendGame(CRoomService* room_service);
    ~InviteFriendGame();
    void InviteGame(int user_id,PlayerInfo&info,int friend_id);
    void IsAgreeInviteGame(int user_id,PlayerInfo&info,int friend_id,bool is_agree,std::string&msg);
    
private:
    std::unordered_map<int32,PlayerInfo>              m_umPlayerInfoDeque       ;  //用于保存用户信息
    CRoomService* m_room_service;
    std::unordered_map<int32,int32>                   m_umInvite;               //<a,b>，a邀请b进行游戏
    
};
#endif /* InviteFriendGame_hpp */
