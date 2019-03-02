//
// Created by zhouwei on 19-3-1.
//

#include "ConutDownLatch.h"


CountDownLatch::CountDownLatch(int count) :
    condition_(mutex_),count_(count)
{
}

void CountDownLatch::wait()
{
    MutexLockGuard lock(mutex_);
    while(count_ > 0)
        condition_.wait();
}

void CountDownLatch::countDown()
{
    MutexLockGuard lock(mutex_);
    --count_;
    if(count_ == 0)
        condition_.notifyall();
}
