//
//  WZQChat.cpp
//  wzqgame
//
//  Created by duole on 2024/2/4.
//

#include "WZQChat.hpp"
#include "gameProtocol.pb.h"
#include "GameService.h"
#include "Table.h"
#include "Player.h"
CWZQChat::CWZQChat(CTable* pTable)
{
    m_pTable = pTable;
}
CWZQChat::~CWZQChat(){}

bool CWZQChat::CheckSensitiveWords(std::string& message)
{
    // 敏感信息列表
    std::string sensitiveWords[] = {"fuck", "笨蛋", "bitch"};
    bool has_sensitive_words = false;
    // 遍历敏感信息列表，检查是否包含敏感词
    for (const auto& word : sensitiveWords) {
        size_t pos = message.find(word);
        if (pos != std::string::npos) {
            // 如果包含敏感词，将其替换为 *
            message.replace(pos, word.length(), word.length(), '*');
            has_sensitive_words = true;
        }
    }

    return has_sensitive_words;
}

void CWZQChat::Chat(std::string message,int chat_id,int send_id)
{
    //检测message是否有敏感信息
    printf("要发送的消息:%s\n",message.c_str());
    bool has_sensitive_words = CheckSensitiveWords(message);
    printf("去敏感词的消息:%s\n",message.c_str());
    
    ACKChatText ack;
    ack.set_is_sensitive(has_sensitive_words);
    ack.set_text(message);
    ack.set_user_id(chat_id);
    ack.set_opp_id(send_id);
    
    
    
    
    auto& table = m_pTable;
    ack.set_table_id(table -> m_nTableID);
    auto& p_gameService = table -> m_pService;
    auto& p_playerMgr = p_gameService->m_pPlayerMgr;
    
    
    std::shared_ptr<CPlayer> player1 = p_playerMgr->m_playersMap[chat_id];
    std::shared_ptr<CPlayer> player2 = p_playerMgr->m_playersMap[send_id];
    

    int64 client_id1 = player1 -> m_nClientID;
    int64 client_id2 = player2 -> m_nClientID;
    
    p_gameService -> SendSvrdMsg(p_gameService -> GetConnSvrdMsgRoute(), (MSGID_CHAT | ID_ACK), 0, 0, ack.SerializeAsString(), client_id1);
    p_gameService -> SendSvrdMsg(p_gameService -> GetConnSvrdMsgRoute(), (MSGID_CHAT | ID_ACK), 0, 0, ack.SerializeAsString(), client_id2);
}
