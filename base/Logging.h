//
// Created by zhouwei on 19-3-4.
//

#ifndef SERVER01_LOGGING_H
#define SERVER01_LOGGING_H


#include "LogStream.h"

class Logger {

public:
    Logger(const char* filename,int line);
    ~Logger();
    LogStream& stream();
    static void setLogFileName(std::string fileName);
    static std::string getLogFIleName();


private:
    class Impl
    {
    public:
        Impl(const char* filename,int line);
        void formatTime();
        LogStream stream_;
        int line_;
        std::string basename_;
    };
    Impl impl_;
    static std::string logFIleName_;
};

#define LOG Logger(__FILE__,__LINE__).stream()


#endif //SERVER01_LOGGING_H
