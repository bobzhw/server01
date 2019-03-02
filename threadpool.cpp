//
// Created by zhouwei on 19-2-28.
//

#include "threadpool.h"
#include<assert.h>
#include<sstream>
#include<iostream>

threadpool::threadpool(const string &name)
    :name_(name),maxQueueSize_(0),running_(false)
{
}

threadpool::~threadpool()
{
    if(running_)
        stop();
}

void threadpool::start(size_t numThreads)
{
    assert(threads_.empty());
    running_ = true;
    threads_.reserve(numThreads);
    while((numThreads--)>0)
    {
        thread* t = new thread(&threadpool::runInThread,this);
        threads_.push_back(t);
    }
    if(numThreads==0)
        threadInitCallback_();
}


void threadpool::stop()
{
    {
        lock_guard<mutex> lck(mutex_);
        running_ = false;
        notEmpty_.notify_all();
    }
    for(auto ptr_thread : threads_)
    {
        ptr_thread->join();
    }

}

size_t threadpool::queueSize() const {

//    lock_guard<mutex> lck(mutex_);
    return queue_.size();
}


void threadpool::run(const threadpool::CTask &task)
{
    if(!task)
        return;
    if(threads_.empty())
        task();
    else
    {
        unique_lock<mutex> lck(mutex_);
        notFull_.wait(lck,[&]{return !isFull();});
        queue_.push_back(task);
        lck.unlock();
        notEmpty_.notify_one();
    }
}

threadpool::CTask threadpool::take()
{
    unique_lock<mutex> lck(mutex_);
    notEmpty_.wait(lck,[&]{return !queue_.empty() || !running_;});
    CTask task;
    if(!queue_.empty())
    {
        task = queue_.front();
        queue_.pop_front();
        if(maxQueueSize_ > 0)
        {
            lck.unlock();
            notFull_.notify_one();
        }
    }
    return task;
}

bool threadpool::isFull() const {
    return maxQueueSize_ > 0 && queue_.size() > maxQueueSize_;
}

void threadpool::runInThread() {
    try {
        if(threadInitCallback_)
            threadInitCallback_();
        while(running_)
        {
            CTask task(take());
            if(task)
                task();
        }
    }
    catch(...)
    {
        fprintf(stderr,"unknown exception caught in threadpool%s\n",name().c_str());
        throw;
    }

}