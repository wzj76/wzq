//
//  WZQChat.hpp
//  wzqgame
//
//  Created by duole on 2024/2/4.
//

#ifndef WZQChat_hpp
#define WZQChat_hpp

#include <stdio.h>

#include <iostream>

class CTable;
class CWZQChat
{
public :
    CWZQChat(CTable* pTable);
    ~CWZQChat();

    void Chat(std::string message,int chat_id,int send_id);

    
private:
    bool CheckSensitiveWords(std::string& message);
    
    CTable* m_pTable;
    

  
};




#endif /* WZQChat_hpp */
