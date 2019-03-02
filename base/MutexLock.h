//
// Created by zhouwei on 19-3-1.
//

#ifndef SERVER01_MUTEXLOCK_H
#define SERVER01_MUTEXLOCK_H

#include <boost/utility.hpp>
#include<pthread.h>
#include<cstdio>

class MutexLock : boost::noncopyable{
public:
    MutexLock()
    {
        int r = pthread_mutex_init(&mutex_,nullptr);
    }
    ~MutexLock()
    {
        pthread_mutex_unlock(&mutex_);
        pthread_mutex_destroy(&mutex_);
    }
    void lock()
    {
        pthread_mutex_lock(&mutex_);
    }
    void unlock()
    {
        pthread_mutex_unlock(&mutex_);
    }
    pthread_mutex_t* get()
    {
        return &mutex_;
    }


private:
    pthread_mutex_t mutex_;

};

class MutexLockGuard : boost::noncopyable
{
public:
    explicit MutexLockGuard(MutexLock& mutex):
        mutex_(mutex)
    {
        mutex.lock();
    }
    ~MutexLockGuard()
    {
        mutex_.unlock();
    }

private:
    MutexLock &mutex_;
};



#endif //SERVER01_MUTEXLOCK_H
