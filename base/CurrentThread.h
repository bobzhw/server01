//
// Created by zhouwei on 19-3-2.
//

#ifndef SERVER01_CURRENTTHREAD_H
#define SERVER01_CURRENTTHREAD_H

#include <cstdint>

namespace CurrentThread
{
    extern __thread int t_cashedTid;
    extern __thread char t_tidString[32];
    extern __thread int t_tidStringLength;
    extern __thread const char* t_threadName;
    void cacheTid();
    inline int tid()
    {
        if(__builtin_expect(t_cashedTid == 0,0))
            cacheTid();
        return t_cashedTid;
    }

    inline const char* tidString()
    {
        return t_tidString;
    }
    inline int tidStringLength()
    {
        return t_tidStringLength;
    }

    inline const char* name()
    {
        return t_threadName;
    }
}

#endif //SERVER01_CURRENTTHREAD_H
