//
//  PlayerInfo.hpp
//  wzqroom
//
//  Created by duole on 2024/1/26.
//

#ifndef PlayerInfo_hpp
#define PlayerInfo_hpp
#include "wzqProtocol.pb.h"
#include <stdio.h>

using google::protobuf::int32;
using google::protobuf::int64;
class PlayerInfo
{
public:
    int32   m_userid;
    UserInfo m_userinfo;
    int64 m_clientid;
    
};

#endif /* PlayerInfo_hpp */
