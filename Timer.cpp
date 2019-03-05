//
// Created by zhouwei on 19-3-4.
//

#include "Timer.h"


time_t TimerNode::getNoWTime(int timeout)
{
    timeval now;
    gettimeofday(&now,nullptr);
    return (((now.tv_sec % 10000) * 1000) + (now.tv_usec / 1000)) + timeout;
}
TimerNode::TimerNode(std::shared_ptr<HttpData>& requestData, int timeout)
    :   deleted_(false),
        SPHttpData(requestData)
{
    expiredTime_ = getNoWTime();
}

TimerNode::TimerNode(TimerNode &tn)
    :   SPHttpData(tn.SPHttpData)
{}
//TO-DO
TimerNode::~TimerNode()
{

}

void TimerNode::update(int timeout)
{
    expiredTime_ = getNoWTime(timeout);
}

bool TimerNode::isValid()
{
    time_t temp = getNoWTime(0);
    if(temp < expiredTime_)
        return true;
    setDeleted();
    return false;
}
//TO-DO
void TimerNode::clearReq()
{

}


TimerManager::TimerManager() {}
TimerManager::~TimerManager() {}

//TO-DO
void TimerManager::addTimer(std::shared_ptr<HttpData> SPHttpData, int timeout)
{
    SPTimerNode node(new TimerNode(SPHttpData,timeout));
    timerNodeQueue_.push(node);
    //TO-DO
}

void TimerManager::handleExpiredEvent()
{
    while(!timerNodeQueue_.empty())
    {
        SPTimerNode node = timerNodeQueue_.top();
        if(node->isDeleteed() || !node->isValid())
            timerNodeQueue_.pop();
        else
            break;
    }
}