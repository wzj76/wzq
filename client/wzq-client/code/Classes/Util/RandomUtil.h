#pragma once

namespace util
{
    
    class RandomUtil
    {
    public:
        //通过输入概率计算是否发生
        static bool RandomProbability(float fProbability);
        
        //取随机数
        static int RandomNumber(int nFrom, int nTo);
        
    };
    
}

