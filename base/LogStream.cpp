//
// Created by zhouwei on 19-3-2.
//

#include "LogStream.h"
#include<cstring>
#include<algorithm>
const char digits[] = "9876543210123456789";
const char* zero = digits+9;
LogStream& LogStream::operator <<(bool v)
{
    buffer_.append(v ? "1" : "0",1);
    return *this;
}

LogStream& LogStream::operator <<(short value)
{
    *this << static_cast<int>(value);
    return *this;
}
LogStream& LogStream::operator <<(unsigned short value)
{
    *this << static_cast<unsigned int>(value);
    return *this;
}
LogStream& LogStream::operator <<(int value)
{
    formatInteger(value);
    return *this;
}
LogStream& LogStream::operator <<(unsigned int value)
{
    formatInteger(value);
    return *this;
}
LogStream& LogStream::operator <<(long value)
{
    formatInteger(value);
    return *this;
}
LogStream& LogStream::operator <<(long long value)
{
    formatInteger(value);
    return *this;
}
LogStream& LogStream::operator <<(unsigned long value)
{
    formatInteger(value);
    return *this;
}
LogStream& LogStream::operator <<(unsigned long long value)
{
    formatInteger(value);
    return *this;
}

LogStream& LogStream::operator <<(float value)
{
    *this << static_cast<double>(value);
    return *this;
}
LogStream& LogStream::operator <<(double value)
{
    if(buffer_.avail() >= kMaxNumericSize)
    {
        int len = snprintf(buffer_.current(),kMaxNumericSize,"%.12g",value);
        buffer_.add(static_cast<size_t>(len));
    }
    return *this;
}
LogStream& LogStream::operator <<(long double value)
{
    if(buffer_.avail() >= kMaxNumericSize)
    {
        int len = snprintf(buffer_.current(),kMaxNumericSize,"%.12Lg",value);
        buffer_.add(static_cast<size_t>(len));
    }
    return *this;
}
LogStream& LogStream::operator <<(char value)
{
     buffer_.append(&value,1);
     return *this;
}
LogStream& LogStream::operator <<(const char* value)
{
    if(value)
        buffer_.append(value,strlen(value));
    else
        buffer_.append("(null)",0);
    return *this;
}
LogStream& LogStream::operator <<(const unsigned char* value)
{
    return operator<<(reinterpret_cast<const char*>(value));
}
LogStream& LogStream::operator <<(const std::string value)
{
    return operator<<(value.c_str());
}


void LogStream::append(const char* data,int len)
{
    buffer_.append(data,len);
}

void LogStream::resetBuffer()
{
    buffer_.reset();
}
//这里的目标是把数字转换为C风格字符串
template <typename T>
size_t convert(char buf[],T value)
{
    T i = value;
    char *p = buf;
    do{
        int lsd = static_cast<int>(i%10);
        i/=10;
        *p++ = zero[lsd];
    }while(i!=0);
    if(value < 0)
        *p++ = '-';
    *p='\0';
    std::reverse(buf,p);
    return p-buf;
}
template <typename T>
void LogStream::formatInteger(T t)
{
    if(buffer_.avail() >= kMaxNumericSize)
    {
        //
        size_t len = convert(buffer_.current(),t);
        buffer_.add(len);
    }

}

