//
// Created by zhouwei on 19-3-2.
//

#ifndef SERVER01_LOGFILE_H
#define SERVER01_LOGFILE_H
#include <boost/noncopyable.hpp>
#include<string>
#include "MutexLock.h"
#include<memory>
#include"FIleUtil.h"
class LogFile : boost::noncopyable{

public:
    LogFile(const std::string& basename,int flushEveryN = 1024);
    ~LogFile();


    void append(const char* logline,size_t len);
    void flush();

private:
    const std::string basename_;
    const int flushEveryN_;

    int count_;
    std::unique_ptr <MutexLock> mutex_;
    std::unique_ptr<AppendFile> file_;

private:
    void append_unlocked(const char* logline,size_t len);

};


#endif //SERVER01_LOGFILE_H
