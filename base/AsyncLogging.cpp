//
// Created by zhouwei on 19-3-3.
//

#include "AsyncLogging.h"
#include<functional>
#include <cassert>
#include"LogFile.h"

AsyncLogging::AsyncLogging(const std::string logFileName, int flushInterval)
    :   flushInterval_(flushInterval),
        running_(false),
        basename_(logFileName),
        thread_(bind(&AsyncLogging::threadFuc,this),"Logging"),
        mutex_(),
        cond_(mutex_),
        currentBuffer_(new Buffer),
        nextBuffer_(new Buffer),
        buffers_(),
        latch_(1)

{
    assert(logFileName.size() > 1);
    currentBuffer_->bzero();
    nextBuffer_->bzero();
    buffers_.reserve(16);

}

/*

该函数是对当前的日志信息加入缓冲区的函数，内部需要判断当前缓冲区容量是否够用，
 如果够用则直接加入当前缓存中，如果不够用则将当前缓存加入缓存容器中，
 重置当前缓存区并与下一块缓存区调换，再将当前信息加入缓存中即可.
*/
void AsyncLogging::append(const char *logline, int len)
{
    MutexLockGuard lock(mutex_);
    if(currentBuffer_->avail() > len)
        currentBuffer_->append(logline,len);
    else
    {
        buffers_.push_back(currentBuffer_);
        currentBuffer_.reset();
        if(nextBuffer_)
            currentBuffer_ = std::move(nextBuffer_);
        else
            currentBuffer_.reset(new Buffer);
        currentBuffer_->append(logline,len);
        cond_.notify();

    }

}

AsyncLogging::~AsyncLogging()
{
    if(running_)
        stop();
}
void AsyncLogging::start()
{
    running_ = true;
    thread_.start();
    latch_.wait();
}
void AsyncLogging::stop()
{
    running_ = false;
    cond_.notify();
    thread_.join();
}

void AsyncLogging::threadFuc() {
    assert(running_);
    latch_.countDown();
    LogFile output(basename_);
    Bufferptr newBuffer1(new Buffer);
    Bufferptr newBuffer2(new Buffer);
    newBuffer1->bzero();
    newBuffer2->bzero();
    BufferVector buffers;
    buffers.reserve(16);
    while (running_) {
        assert(newBuffer1 && newBuffer1->length() == 0);
        assert(newBuffer2 && newBuffer2->length() == 0);
        assert(buffers.empty());
        {
            MutexLockGuard lock(mutex_);
            if (buffers_.empty())
                cond_.waitForSeconds(flushInterval_);
            buffers_.push_back(currentBuffer_);
            currentBuffer_.reset();
            currentBuffer_ = std::move(newBuffer1);
            buffers.swap(buffers_);
            if (!nextBuffer_)
                nextBuffer_ = std::move(newBuffer2);
        }
        assert(!buffers.empty());
        if (buffers.size() > 25)
            buffers.erase(buffers.begin() + 2, buffers.end());

        for (auto &buffer : buffers) {
            output.append(buffer->data(), buffer->length());
        }

        if (buffers.size() > 2)
            buffers.resize(2);
        if (!newBuffer1) {
            assert(!buffers.empty());
            newBuffer1 = buffers.back();
            buffers.pop_back();
            newBuffer1->reset();
        }
        if (!newBuffer2) {
            assert(!buffers.empty());
            newBuffer2 = buffers.back();
            buffers.pop_back();
            newBuffer2->reset();
        }

        buffers.clear();
        output.flush();
    }
    output.flush();
}