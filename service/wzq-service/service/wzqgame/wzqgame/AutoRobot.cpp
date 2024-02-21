//
//  AutoRobot.cpp
//  wzqgame
//
//  Created by duole on 2024/2/20.
//

#include "AutoRobot.hpp"
#include <iostream>
#include <random>
AutoRobot::AutoRobot()
{
}
AutoRobot::AutoRobot(unsigned int boardsize)
{
    m_boardsize = boardsize;
}

std::pair<int,int> AutoRobot::GetkeySetchess(std::vector<std::vector<int>>&board)
{
    std::pair<int, int> ans;
    //机器人为黑棋并且是第一个棋子，下在中间
    if (board[m_boardsize/2][m_boardsize/2] == 0 && m_color_id ==BLACK_CHESS)
    {
       //放到中间
        ans.first = board.size()/2;
        ans.second = board[0].size()/2;
        return ans;
    }
    else
    {

        int score = 0;
        int maxScores = -1;
        int maxX = 0;
        int maxY = 0;
        std::vector<std::pair<int,int>> v_pos_pair;
        for(int i=0;i<board.size();i++)
        {
            for(int j=0;j<board[0].size();j++)
            {
                //这个位置已经下过棋
                if(board[i][j]!=0)
                {
                    continue;
                }
                score = GetAllScores(board,i, j);
                /*获取最大的得分点*/
                if (score > maxScores)
                {
                    maxScores = score;
                    maxX = i;
                    maxY = j;
                    v_pos_pair.push_back({i,j});
                    std::cout<<"放入找到得分点坐标:"<<i<<" "<<j<<" 分数:"<<score<<std::endl;
                }
            }
        }
        if(m_difficulty == KEY_HARD)
        {
            ans.first = maxY;
            ans.second = maxX;
        }
        else if(m_difficulty == KEY_NORMAL)
        {
            std::random_device rd;
            std::mt19937 gen(rd());
            // 定义生成随机数范围为 [1, 2]
            std::uniform_int_distribution<> dis(1, 2);
            int randomNum = dis(gen);
            if(v_pos_pair.size()>=randomNum)
            {
                auto pair = v_pos_pair[v_pos_pair.size()-randomNum];
                ans.first = pair.second;
                ans.second = pair.first;
            }
            else
            {
                ans.first = maxY;
                ans.second = maxX;
            }
        }
        else
        {
            std::random_device rd;
            std::mt19937 gen(rd());
            // 定义生成随机数范围为 [1, 3]
            std::uniform_int_distribution<> dis(1, 3);
            int randomNum = dis(gen);
            
            if(v_pos_pair.size()>=randomNum)
            {
                auto pair = v_pos_pair[v_pos_pair.size()-randomNum];
                ans.first = pair.second;
                ans.second = pair.first;
            }
            else
            {
                ans.first = maxY;
                ans.second = maxX;
            }
        }
        
    }
   
    return ans;
}
bool AutoRobot::isOk(int x, int y)
{
    return x >= 0 && y >= 0 && x < m_boardsize && y < m_boardsize;
    
}
int AutoRobot::GetAllScores(std::vector<std::vector<int>>&board,int x, int y)
{
    int ret = 0;
    int x1(x), y1(y), x2(x), y2(y), x3(x), y3(y), x4(x), y4(y);
    for (int i = 0; i < 5; i++)
    {
        if (isOk(x1, y1))
        {
            ret += CalculateScore(GetScore1(board,x1, y1));
            y1++;
        }
        if (isOk(x2, y2))
        {
            ret += CalculateScore(GetScore2(board,x2, y2));
            x2++;
        }
        if (isOk(x3, y3))
        {
            ret += CalculateScore(GetScore3(board,x3, y3));
            x3--, y3++;
        }
        if (isOk(x4, y4))
        {
            ret += CalculateScore(GetScore4(board,x4, y4));
            x4++, y4++;
        }
    }
    return ret;
}

std::vector<int> AutoRobot::GetScore1(std::vector<std::vector<int>>&board,int x, int y)
{
    int k = 0, t = 0, cnt = 5;
    while (cnt-- && isOk(x,y))
    {
        if (board[x][y] == m_color_id)
        {
            k++;
        }
        if (board[x][y] != 0 && board[x][y] != m_color_id)
        {
            t++;
        }
        
        y--;
    }
    if (cnt >= 0)    return {};
    return std::vector<int>({ k, t });
}

std::vector<int> AutoRobot::GetScore2(std::vector<std::vector<int>>&board,int x, int y)
{
    int k = 0, t = 0, cnt = 5;
    while (cnt-- && isOk(x, y))
    {
        if (board[x][y] == m_color_id)
        {
            k++;
        }
        if (board[x][y] != 0 && board[x][y] != m_color_id)
        {
            t++;
        }
        x--;
    }
    if (cnt >= 0 )    return {};
    return std::vector<int>({ k, t });
}

std::vector<int> AutoRobot::GetScore3(std::vector<std::vector<int>>&board,int x, int y)
{
    int k = 0, t = 0, cnt = 5;
    while (cnt-- && isOk(x, y))
    {
        if (board[x][y] == m_color_id)
        {
            k++;
        }
        if (board[x][y] != 0 && board[x][y] != m_color_id)
        {
            t++;
        }
        x++, y--;
    }
    if (cnt >= 0)
    {
        return {};
    }
    return std::vector<int>({k,t});
}

std::vector<int> AutoRobot::GetScore4(std::vector<std::vector<int>>&board,int x, int y)
{
    int k = 0, t = 0, cnt = 5;
    while (cnt-- && isOk(x, y))
    {
        if (board[x][y] == m_color_id)
        {
            k++;
        }
        if (board[x][y] != 0 && board[x][y] != m_color_id)
        {
            t++;
        }
        x--, y--;
    }
    if (cnt >= 0)
    {
        return {};
    }
    return std::vector<int>({ k,t });
}

int AutoRobot::CalculateScore(std::vector<int> nums)
{
    if (nums.empty())
    {
        return 0;
    }
    
    int user=0, robot=0;

    user = nums[0], robot = nums[1];//user和robot的得分
    

    if (user == 0 && robot == 0)
    {
        return 10;
    }
    else if (user == 1 && robot == 0)
    {
        return 35;
    }
    else if (user == 2 && robot == 0)
    {
        return 1500;
    }
    else if (user == 3 && robot == 0)
    {
        return 18000;
    }
    else if (user == 4 && robot == 0)
    {
        return 1000000;
    }
    else if (user == 0 && robot == 1)
    {
        return 15;
    }
    else if (user == 0 && robot == 2)
    {
        return 400;
    }
    else if (user == 0 && robot == 3)
    {
        return 6000;
    }
    else if (user == 0 && robot == 4)
    {
        return 150000;
    }
    else if (user != 0 && robot != 0)
    {
        return 0;
    }
    return 0;
}
