//
// Created by zhouwei on 19-3-2.
//

#ifndef SERVER01_FILEUTIL_H
#define SERVER01_FILEUTIL_H
#include<boost/noncopyable.hpp>
#include<string>

class AppendFile {
public:
    explicit AppendFile(std::string filename);
    ~AppendFile();
    void append(const char* logline,const size_t len);
    void flush();

private:
    size_t write(const char* logline,size_t len);
    FILE* fp;
    char buffer[64*1024];
};


#endif //SERVER01_FILEUTIL_H
