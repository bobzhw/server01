//
// Created by zhouwei on 19-2-28.
//

#pragma once

#include<functional>
#include<thread>
#include<vector>
#include<mutex>
#include<condition_variable>
#include<deque>
#include <boost/core/noncopyable.hpp>

using namespace std;
class threadpool : boost::noncopyable{
public:
    //
    typedef function<void()> CTask;
    explicit threadpool(const string& name = string("ThreadPool"));
    ~threadpool();


    void setMaxQueueSize(size_t maxSize)
    {
        maxQueueSize_ = maxSize;
    }
    void setThreadInitCallback(const CTask& cb)
    {
        threadInitCallback_ = cb;
    }

    void start(size_t numThreads);

    void stop();

    const string& name() const
    {
        return name_;
    }

    size_t queueSize() const;

    void run(const CTask& task);

private:

    bool isFull() const ;

    void runInThread();
    CTask take();

private:
    size_t maxQueueSize_;
    CTask threadInitCallback_;
    string name_;
    bool running_;
    vector<thread*> threads_;
    mutable mutex mutex_;
    condition_variable notEmpty_;
    condition_variable notFull_;
    deque<CTask> queue_;
};



