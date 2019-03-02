//
// Created by zhouwei on 19-3-1.
//

#include "Condition.h"
#include<time.h>
#include <cerrno>

Condition::Condition(MutexLock &_mutex) :
    mutex_(_mutex)
{
    pthread_cond_init(&cond_, nullptr);
}

Condition::~Condition()
{
    pthread_cond_destroy(&cond_);
}

void Condition::wait()
{
    pthread_cond_wait(&cond_,mutex_.get());
}

void Condition::notify()
{
    pthread_cond_signal(&cond_);
}

void Condition::notifyall()
{
    pthread_cond_broadcast(&cond_);
}

bool Condition::waitForSeconds(int seconds)
{
    timespec abstime;
    clock_gettime(CLOCK_REALTIME,&abstime);
    abstime.tv_sec += static_cast<time_t>(seconds);
    return ETIMEDOUT == pthread_cond_timedwait(&cond_,mutex_.get(),&abstime);

}