//
// Created by zhouwei on 19-3-4.
//

#ifndef SERVER01_CHANNEL_H
#define SERVER01_CHANNEL_H
#include<functional>
#include<memory>

class EventLoop;
class HttpData;

class Channel {

private:
    typedef std::function<void()> CallBack;
    EventLoop *loop_;
    int fd_;
    __uint32_t events_;
    __uint32_t revents_;
    __uint32_t lastEvents_;

    std::weak_ptr<HttpData> holder_;

private:
    CallBack readHandler_;
    CallBack writeHandler_;
    CallBack errorHandler_;
    CallBack connHandler_;


public:
    Channel(EventLoop* loop);
    Channel(EventLoop* loop,int fd);
    ~Channel();
    int getFd() const;
    void setFd(int fd);

    void setHoder(std::shared_ptr<HttpData> holder);
    std::shared_ptr<HttpData> getHolder();

    void setReadHandler(CallBack &&readHandler);
    void setWriteHandler(CallBack &&writeHandler);
    void setErrorHandler(CallBack &&errorHandler);
    void setConnHandler(CallBack &&connHandler);

    void handlerEvents();
    void handlerRead();
    void handlerWrite();
    void handlerError();
    void handlerConn();

    void setRevents(__uint32_t ev);
    void setEvents(__uint32_t ev);
    __uint32_t getEvents() const;
    bool equalAndUpdateLastEvnets();
    __uint32_t getLastEvnets() const;

};


typedef std::shared_ptr<Channel> SP_Channel;


#endif //SERVER01_CHANNEL_H
