//
// Created by zhouwei on 19-3-1.
//

#include "Thread.h"
#include<cassert>
#include <syscall.h>
#include"CurrentThread.h"
#include<sys/prctl.h>

Thread::Thread(const Thread::ThreadFunc &func, const std::string &name)
    :started_(false),
    joined_(false),
    pthreadId_(0),
    tid_(0),
    name_(name),
    func_(func),
    latch_(1)
{
}

Thread::~Thread()
{
    if(started_ && !joined_)
        pthread_detach(pthreadId_);
}

void *startThread(void* o)
{
    auto* data = static_cast<ThreadData*>(o);
    data->runInThread();
    delete data;
    return nullptr;
}

void Thread::start()
{
    assert(!started_);
    started_ = true;
    auto data = new ThreadData(func_,name_,&tid_,&latch_);
    if(pthread_create(&pthreadId_, nullptr,&startThread,data))
    {
        started_ = false;
        delete data;
    }
}

int Thread::join()
{
    assert(started_);
    assert(!joined_);
    joined_ = true;
    return pthread_join(pthreadId_, nullptr);
}

void ThreadData::runInThread()
{
    *tid_ = CurrentThread::tid();
    tid_ = nullptr;
    latch_->countDown();
    latch_ = nullptr;

    CurrentThread::t_threadName = name_.empty() ? "Thread" : name_.c_str();
    prctl(PR_SET_NAME,CurrentThread::t_threadName);

    func_();
    CurrentThread::t_threadName = "finished";
}

namespace  CurrentThread
{
    __thread int t_cashedTid = 0;
    __thread char t_tidString[32];
    __thread int t_tidStringLength = 6;
    __thread const char* t_threadName = "default";
}
pid_t getId()
{
    return static_cast<pid_t >(syscall(SYS_gettid));
}

void CurrentThread::cacheTid()
{
    if(t_cashedTid == 0)
    {
        t_cashedTid = getId();

    }
}

ThreadData::ThreadData(const ThreadData::ThreadFunc &func,
        const std::string &name,
        pid_t *tid,
        CountDownLatch *latch):
        func_(func),name_(name),tid_(tid),latch_(latch)
{
}