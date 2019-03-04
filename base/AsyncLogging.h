//
// Created by zhouwei on 19-3-3.
//

#ifndef SERVER01_ASYNCLOGGING_H
#define SERVER01_ASYNCLOGGING_H
#include<boost/noncopyable.hpp>
#include<string>
#include "LogStream.h"
#include<vector>
#include <memory>
#include"Thread.h"
//AsyncLogging是核心，它负责启动一个log线程，专门用来将log写入LogFile，
//应用了“双缓冲技术”，其实有4个以上的缓冲区，但思想是一样的。
//AsyncLogging负责(定时到或被填满时)将缓冲区中的数据写入LogFile中。
class AsyncLogging : boost::noncopyable
{
public:
    AsyncLogging(const std::string basename,int flushInterval =2);
    ~AsyncLogging();
    void append(const char* logline,int len);
    void start();
    void stop();

private:
    void threadFuc();
private:
    typedef FixedBuffer<kLargeBuffer> Buffer;
    typedef std::vector<std::shared_ptr<Buffer>> BufferVector;
    typedef std::shared_ptr<Buffer> Bufferptr;

private:
    const int flushInterval_;
    bool running_;
    std::string basename_;
    Thread thread_;
    MutexLock mutex_;
    Condition cond_;
    Bufferptr currentBuffer_;
    Bufferptr nextBuffer_;
    BufferVector buffers_;
    CountDownLatch latch_;

};


#endif //SERVER01_ASYNCLOGGING_H
