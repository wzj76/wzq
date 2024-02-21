//
//  ThreadPool.hpp
//  wzqgame
//
//  Created by duole on 2024/2/6.
//

#ifndef ThreadPool_hpp
#define ThreadPool_hpp

#include <cstdio>
#include<iostream>
#include<mutex>
#include<string>
#include<thread>
#include<future>
#include<queue>//任务队列，生产者
#include<vector>
#include<functional>
using namespace std;

class Threadpool
{
public:
    Threadpool(int numThreads) :stop(false)
    {
        for (int i = 0; i < numThreads; i++)
        {
            threads.emplace_back([this]() {//这个emplace_back比push_back的效率要高不少，没有调用拷贝构造再复制一个新的容器，而是直接就地构造
                while (1) {
                    std::unique_lock<std::mutex>lock(mtx);//智能锁
                    condition.wait(lock, [this]() {
                        return !tasks.empty() || stop;
                        });//阻塞的条件是整个程序不终止或者消息队列为空
                    if (stop && tasks.empty()) {//还要判断任务队列是否为空
                        return;
                    }
                    
                    std::function<void()>task(std::move(tasks.front()));//取到队列里面最左边的任务
                    tasks.pop();
                    lock.unlock();//这里有个疑问，不应该是智能锁吗？
                    task();
                }

                });//这个更加节省资源

        }

    }

    ~Threadpool()
    {
        {
            std::unique_lock <std::mutex> lock(mtx);
            stop = true;//代表线程停止了
        }
        condition.notify_all();//把线程队列里面所有任务完成
        for (auto& t : threads)
        {
            t.join();
        }
    }

    template <class F, class ... Args>//可变参数模板
    void enqueue(F&& f, Args&&...args)//在函数模板里面使用右值引用相当于万能引用（既可以当左值也可以当右值）
    {
            std::function<void()>task = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
        {
            std::unique_lock<std::mutex>lock(mtx);//这里要指定这个锁的范围
            tasks.emplace(move(task));
        }
        condition.notify_one();
    }

private:
    vector<std::thread> threads;//工作的线程数组
    queue < std::function<void()>>tasks;//任务队列（属于函数类型）
    std::mutex mtx;//控制线程进出（需要一个互斥锁+）
    std::condition_variable condition;//条件变量
    bool stop;
};

#endif /* ThreadPool_hpp */
