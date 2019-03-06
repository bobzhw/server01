//
// Created by zhouwei on 19-3-4.
//

#ifndef SERVER01_UTIL_H
#define SERVER01_UTIL_H

#include <stdio.h>
#include<cerrno>
#include<sys/socket.h>
#include<unistd.h>
#include<string>
#include<signal.h>
#include<cstring>
#include<fcntl.h>
#include<netinet/in.h>
#include<netinet/tcp.h>
class Util {

public:
    ssize_t static readn(int fd,void *buf,size_t n)
    {
        size_t nleft = n;
        ssize_t nread = 0;
        ssize_t readSum =0;
        char* ptr = static_cast<char*>(buf);
        while(nleft > 0)
        {
            if((nread = read(fd,ptr,nleft)) < 0)
            {
                if(errno == EINTR)
                {
                    nread = 0;
                    continue;
                }

                else if(errno == EAGAIN)
                    return readSum;
                return -1;
            }
            else if(nread == 0)
                break;
            nleft -= nread;
            ptr += nread;

        }
        return n-nleft;
    }

    ssize_t static readn(int fd,std::string &inBuffer)
    {
        size_t size = inBuffer.size();
        const char* buf = inBuffer.c_str();
        return readn(fd, static_cast<void*>(const_cast<char*>(buf)),size);
    }

    ssize_t static writen(int fd,void* buf,size_t n)
    {
        size_t nleft = n;
        ssize_t nwritten = 0;
        ssize_t writeSum = 0;
        char* ptr = static_cast<char*>(buf);
        while(nleft>0)
        {
            if((nwritten = write(fd,ptr,nleft)) <= 0)
            {
                if(nwritten < 0)
                {
                    if(errno == EINTR)
                    {
                        nwritten = 0;
                    }
                    else if(errno == EAGAIN)
                        return writeSum;
                    else
                        return -1;

                }
            }
            writeSum += nwritten;
            nleft -= nwritten;
            ptr += nwritten;
        }
        return writeSum;
    }

    ssize_t static writen(int fd,std::string &buf)
    {
        const char* ch = buf.c_str();
        size_t size = buf.size();
        return writen(fd,static_cast<void*>(const_cast<char*>(ch)),size);

    }

    void static handle_for_sigpipe()
    {
        struct sigaction sa;
        memset(&sa,'\0', sizeof(sa));
        sa.sa_handler = SIG_IGN;
        sa.sa_flags = 0;
        if(sigaction(SIGPIPE,&sa, nullptr))
            return;
    }

    int static setSocketNonBlocking(int fd)
    {
        int flag = fcntl(fd,F_GETFL,0);
        if(flag == -1)
            return -1;
        flag |= O_NONBLOCK;
        if(fcntl(fd,F_SETFL,flag) == -1)
            return -1;
        return 0;
    }

    void static setSocketNoDelay(int fd)
    {
        int enable = 1;
        setsockopt(fd,IPPROTO_TCP,TCP_NODELAY,
                   (void*)&enable, sizeof(enable));
    }

    void static shutDownWR(int fd)
    {
        shutdown(fd,SHUT_WR);
    }

    int static socket_bind_listen(int port)
    {
        if(port < 0 || port > 65535)
            return -1;

        int lfd = 0;
        if((lfd = socket(AF_INET,SOCK_STREAM,0)) == -1)
            return -1;

        int optval = 1;
        if(setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval)) == -1)
            return -1;

        sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        server_addr.sin_port = htons((unsigned short)port);

        if(bind(lfd,(struct sockaddr*)&server_addr,sizeof(server_addr)) == -1)
            return -1;
        if(listen(lfd,2048) == -1)
            return -1;
        if(lfd == -1)
        {
            close(lfd);
            return -1;
        }
        return lfd;

    }
};




#endif //SERVER01_UTIL_H
