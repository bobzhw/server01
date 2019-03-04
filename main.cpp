#include <iostream>
#include"base/Thread.h"
#include "base/LogStream.h"
#include"base/AsyncLogging.h"
using namespace std;
void* dosomework1()
{
    cout << "gan" << endl;
}

int main()
{
    AsyncLogging as("name");
    as.start();
    for(int i = 0;i<10;i++)
    {
        char* c = "wocaonidayede wocao";
        size_t s = strlen(c);
        as.append(c,s);
    }
    sleep(1);
}