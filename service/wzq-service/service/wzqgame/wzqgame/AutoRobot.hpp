//
//  AutoRobot.hpp
//  wzqgame
//
//  Created by duole on 2024/2/20.
//

#ifndef AutoRobot_hpp
#define AutoRobot_hpp

#include <stdio.h>
#include <vector>
#include <string>
#define BOARD_SIZE 13
#define BLACK_CHESS 1
#define WHITE_CHESS 2
#define KEY_EASY "easymode"
#define KEY_NORMAL "normalmode"
#define KEY_HARD "hardmode"
class AutoRobot
{
public:
    AutoRobot();
    AutoRobot(unsigned int boardsize);
    //选择合适位置下棋
    std::pair<int,int> GetkeySetchess(std::vector<std::vector<int>>&board);
    

    int GetAllScores(std::vector<std::vector<int>>&board,int x, int y);
    

    std::vector<int> GetScore1(std::vector<std::vector<int>>&board,int x, int y);
    

    std::vector<int> GetScore2(std::vector<std::vector<int>>&board,int x, int y);

    std::vector<int> GetScore3(std::vector<std::vector<int>>&board,int x, int y);

    std::vector<int> GetScore4(std::vector<std::vector<int>>&board,int x, int y);

    int CalculateScore(std::vector<int> nums);
    bool isOk(int x, int y);
    
    int m_color_id;
    std::string m_difficulty = KEY_EASY;
    unsigned int m_boardsize;
};

#endif /* AutoRobot_hpp */
