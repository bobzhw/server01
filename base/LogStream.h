//
// Created by zhouwei on 19-3-2.
//

#ifndef SERVER01_LOGSTREAM_H
#define SERVER01_LOGSTREAM_H
#include<boost/noncopyable.hpp>
#include<cstring>
#include<string>
const int kSmallBuffer = 4096;
const int kLargeBuffer = 4096*1024;

template <int SIZE>
class FixedBuffer : boost::noncopyable
{
public:
    FixedBuffer() : cur_(data_){}
    ~FixedBuffer() = default;

    void append(const char* buf,size_t len)
    {
        if(avail() > static_cast<int>(len))
        {
            memcpy(cur_,buf,len);
            cur_+=len;
        }
    }

    const char* data() const{return data_;}
    int length() const{return static_cast<int>(cur_-data_);}
    char* current(){return cur_;}
    int avail() const{return static_cast<int>(end()-cur_);}
    void add(size_t len){cur_+= len;}
    void reset(){cur_=data_;}
    void bzero(){memset(data_,0,sizeof(data_));}

private:
    const char* end() const{return data_ + sizeof(data_);}
    char data_[SIZE];
    char* cur_;
};

class LogStream : boost::noncopyable
{
public:
    typedef FixedBuffer<kSmallBuffer> Buffer;
    //override operator
    LogStream&operator <<(bool v);
    LogStream&operator <<(short);
    LogStream&operator <<(unsigned short);
    LogStream&operator <<(int);
    LogStream&operator <<(unsigned int);
    LogStream&operator <<(long);
    LogStream&operator <<(long long);
    LogStream&operator <<(unsigned long);
    LogStream&operator <<(unsigned long long);

    LogStream&operator <<(float);
    LogStream&operator <<(double);
    LogStream&operator <<(long double);

    LogStream&operator <<(char);
    LogStream&operator <<(const char*);
    LogStream&operator <<(const unsigned char*);
    LogStream&operator <<(const std::string);


    void append(const char* data,int len);
    const Buffer& buffer() const {return buffer_;}
    void resetBuffer();


private:
    Buffer buffer_;

    template <typename T>
    void formatInteger(T);


    static const int kMaxNumericSize = 32;

};


#endif //SERVER01_LOGSTREAM_H
