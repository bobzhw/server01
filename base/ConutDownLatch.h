//
// Created by zhouwei on 19-3-1.
//

#ifndef SERVER01_CONUTDOWNLATCH_H
#define SERVER01_CONUTDOWNLATCH_H
#include "Condition.h"
#include "MutexLock.h"
#include<thread>
#include <boost/utility.hpp>
using namespace std;
class CountDownLatch : boost::noncopyable{
public:
    explicit CountDownLatch(int count);
    void wait();
    void countDown();

private:
   mutable MutexLock mutex_;
   Condition condition_;
   int count_;


};


#endif //SERVER01_CONUTDOWNLATCH_H
