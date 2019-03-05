//
// Created by zhouwei on 19-3-4.
//

#include "Logging.h"
#include"AsyncLogging.h"
#include<time.h>
#include<sys/time.h>
static pthread_once_t once_control_ = PTHREAD_ONCE_INIT;
static AsyncLogging *AsyncLogger_;

std::string Logger::logFIleName_ = "zw_log.log";


void once_init()
{
    AsyncLogger_ = new AsyncLogging(Logger::getLogFIleName());
    AsyncLogger_->start();
}

void output(const char* msg,int len)
{
    pthread_once(&once_control_,once_init);
    AsyncLogger_->append(msg,len);
}
Logger::Logger(const char *filename, int line)
    :   impl_(filename,line)
{}

Logger::~Logger()
{
    Logger::impl_.stream_ << "--" << Logger::impl_.basename_
        << ":" << Logger::impl_.line_ << "\n";
    const LogStream::Buffer& buf(stream().buffer());
    output(buf.data(),buf.length());
}

Logger::Impl::Impl(const char *filename, int line)
    : stream_(),line_(line),basename_(filename)
{
    formatTime();
}


LogStream& Logger::stream()
{
    return impl_.stream_;
}


std::string Logger::getLogFIleName()
{
    return logFIleName_;
}

void Logger::setLogFileName(std::string fileName)
{
    logFIleName_ = fileName;
}



void Logger::Impl::formatTime()
{
    timeval tv;
    time_t time;
    char str_t[26] = {0};
    gettimeofday(&tv, nullptr);
    time = tv.tv_sec;
    tm* p_time = localtime(&time);
    strftime(str_t,26,"%Y-%m-%d %H:%M:%S\n",p_time);
    stream_ << str_t;
}