//
// Created by zhouwei on 19-3-1.
//

#ifndef SERVER01_CONDITION_H
#define SERVER01_CONDITION_H


#include <boost/core/noncopyable.hpp>
#include "MutexLock.h"
class Condition : boost::noncopyable
{
public:
    explicit Condition(MutexLock &_mutex);
    ~Condition();
    void wait();
    void notify();
    void notifyall();
    bool waitForSeconds(int seconds);

private:

    MutexLock &mutex_;
    pthread_cond_t cond_;

};


#endif //SERVER01_CONDITION_H
