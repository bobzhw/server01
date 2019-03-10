//
// Created by zhouwei on 19-3-4.
//

#include "Channel.h"
#include<cstdlib>
#include <sys/epoll.h>


Channel::Channel(EventLoop *loop)
    :   loop_(loop),
        events_(0),
        lastEvents_(9)
{

}

Channel::Channel(EventLoop *loop, int fd)
    :   loop_(loop),
        fd_(fd),
        events_(0),
        lastEvents_(0)
{

}

Channel::~Channel()
{}


int Channel::getFd() const
{
    return fd_;
}

void Channel::setFd(int fd)
{
    fd_ = fd;
}

void Channel::handlerRead()
{
    if(readHandler_)
        readHandler_();
}

void Channel::handlerWrite()
{
    if(writeHandler_)
        writeHandler_();
}

void Channel::handlerConn()
{
    if(connHandler_)
        connHandler_();
}

void Channel::handlerError()
{
    if(errorHandler_)
        errorHandler_();
}

void Channel::setHoder(std::shared_ptr<HttpData> holder)
{
    holder_ = holder;
}

std::shared_ptr<HttpData> Channel::getHolder()
{
    std::shared_ptr<HttpData> ret(holder_.lock());
    return ret;
}

void Channel::setReadHandler(Channel::CallBack &&readHandler)
{
    readHandler = readHandler;
}

void Channel::setWriteHandler(Channel::CallBack &&writeHandler)
{
    writeHandler = writeHandler;
}

void Channel::setErrorHandler(Channel::CallBack &&errorHandler)
{
    errorHandler_ = errorHandler;
}

void Channel::setConnHandler(Channel::CallBack &&connHandler)
{
    connHandler_ = connHandler;
}

void Channel::handlerEvents()
{
    events_ = 0;
    if((revents_ & EPOLLHUP) &&(revents_ & EPOLLIN))
    {
        events_ = 0;
        return;
    }
    if((revents_ & EPOLLERR))
    {
        if(errorHandler_)
            errorHandler_();
        return;
    }
    if(revents_ & (EPOLLIN | EPOLLPRI | EPOLLRDHUP)
    {
        handlerRead();
    }
    if(revents_ & EPOLLOUT)
    {
        handlerWrite();
    }
    handlerConn();
}

void Channel::setRevents(__uint32_t ev)
{
    revents_ = ev;
}

void Channel::setEvents(__uint32_t ev)
{
    events_ = ev;
}

__uint32_t Channel::getEvents() const
{
    return events_;
}

bool Channel::equalAndUpdateLastEvnets()
{
    bool ret = (lastEvents_ == events_);
    lastEvents_ = events_;
    return  ret;
}

__uint32_t Channel::getLastEvnets() const
{
    return lastEvents_;
}

