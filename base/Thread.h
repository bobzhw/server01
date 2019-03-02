//
// Created by zhouwei on 19-3-1.
//

#ifndef SERVER01_THREAD_H
#define SERVER01_THREAD_H
#include<functional>
#include"ConutDownLatch.h"
class Thread {
public:
    typedef std::function<void()> ThreadFunc;
    explicit Thread(const ThreadFunc&,const std::string& name = std::string("Thread"));
    ~Thread();
    void start();
    int join();
    bool isStarted() const
    {
        return started_;
    }
    pid_t tid() const
    {
        return tid_;
    }
    const std::string& name() const
    {
        return name_;
    };


private:
    bool started_;
    pid_t tid_;
    std::string name_;
    bool joined_;
    ThreadFunc func_;
    CountDownLatch latch_;
    pthread_t pthreadId_;
};


class ThreadData : boost::noncopyable
{
public:
    typedef std::function<void()> ThreadFunc;
    ThreadFunc func_;
    std::string name_;
    pid_t* tid_;
    CountDownLatch* latch_;

    ThreadData(const ThreadFunc &func,const std::string& name,
            pid_t *tid,CountDownLatch* latch);
    void runInThread();

};


#endif //SERVER01_THREAD_H
