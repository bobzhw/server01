//
// Created by zhouwei on 19-3-5.
//

#ifndef SERVER01_EVENTLOOP_H
#define SERVER01_EVENTLOOP_H
#include<memory>
#include "Channel.h"
#include "epoll.h"
#include"base/MutexLock.h"
class EventLoop {

public:
    typedef std::function<void()> Functor;
    EventLoop();
    ~EventLoop();
    void loop();
    void quit();
    void runInLoop(Functor &&cb);
    void queueInLoop(Functor &&cb);
    bool isInLoopThread() const ;
    void assertInLoopThread();
    void shutdown(std::shared_ptr<Channel>& channel);
    void removeFromPoller(std::shared_ptr<Channel>& channel);
    void updatePoller(std::shared_ptr<Channel>& channel,int timeout = 0);
    void addToPoller(std::shared_ptr<Channel>& channel,int timeout = 0);

private:
    bool looping_;
    std::shared_ptr<Epool> poller_;
    int wakeupFd_;
    bool quit_;
    bool eventHandling_;
    mutable MutexLock mutex_;
    std::vector<Functor> pendingFunctors_;
    bool callingPendingFunctors_;
    const pid_t threadId_;
    std::shared_ptr<Channel> pwakeupChannel_;

private:
    void wakeup();
    void handleRead();
    void doPendingFunctors();
    void handleConn();
};


#endif //SERVER01_EVENTLOOP_H
