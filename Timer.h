//
// Created by zhouwei on 19-3-4.
//

#ifndef SERVER01_TIMER_H
#define SERVER01_TIMER_H
#include<memory>
#include<queue>
#include<sys/time.h>
class HttpData;

class TimerNode
{
public:
    TimerNode(std::shared_ptr<HttpData>& requestData,int timeout);
    ~TimerNode();
    TimerNode(TimerNode( &tn));
    void update(int timeout);
    bool isValid();

    void clearReq();
    void setDeleted()
    {
        deleted_=true;
    }
    bool isDeleteed() const
    {
        return deleted_;
    }

    time_t getExpTime() const
    {
        return expiredTime_;
    }
private:
    time_t getNoWTime(int timeout);
    bool deleted_;
    time_t expiredTime_;
    std::shared_ptr<HttpData> SPHttpData;

};

class TimerCmp
{
public:
    bool operator()(std::shared_ptr<TimerNode>& lhs,std::shared_ptr<TimerNode>& rhs)
    {
        return lhs->getExpTime() > rhs->getExpTime();
    }
};

class TimerManager
{
public:
    TimerManager();
    ~TimerManager();
    void addTimer(std::shared_ptr<HttpData> SPHttpData,int timeout);
    void handleExpiredEvent();

private:
    typedef std::shared_ptr<TimerNode> SPTimerNode;
    std::priority_queue<SPTimerNode,std::deque<SPTimerNode>,TimerCmp> timerNodeQueue_;
};
#endif //SERVER01_TIMER_H
