//
//  Singleton.h
//  gamebase
//
//  Created by 张建民 on 2021/2/25.
//  Copyright © 2021 Duole Games. All rights reserved.
//
//  单例模板实现:线程安全；
//  usage:
//  CUserModel 单例对象声明：
//  #define USER_INSTANCE duole::CSingleton<duole::CUserModel>::Instance()
//  调用：
//  USER_INSTANCE.doSomethings();

#ifndef Singleton_h
#define Singleton_h

#include <stdlib.h>
#include <stdio.h>
#include <mutex>

namespace duole
{
template <typename T>
class CSingleton
{
public:
    static T& Instance()
    {
        static std::mutex mtx;
        if (NULL == m_pInstance)
        {
            // Lock lock;
            std::lock_guard<std::mutex> lck(mtx);
            if (NULL == m_pInstance)
            {
                m_pInstance = new T();
                atexit(Destroy);
            }
            return *m_pInstance;
        }
        return *m_pInstance;
    }
protected:
    CSingleton(void) {}
    ~CSingleton(void) {}
private:
    CSingleton(const CSingleton& rhs) {}
    CSingleton& operator = (const CSingleton& rhs) {}

    static void Destroy()
    {
        if (NULL != m_pInstance)
        {
            delete m_pInstance;
        }
        m_pInstance = NULL;
    }
    static T* volatile m_pInstance;

};
template <typename T>
T* volatile CSingleton<T>::m_pInstance = NULL;

} // namespace duole

#endif /* Singleton_h */
