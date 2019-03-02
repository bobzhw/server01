#include <iostream>
#include"base/Thread.h"

using namespace std;
void* dosomework1()
{
    cout << "gan" << endl;
}

int main()
{
    Thread* t = new Thread(dosomework1);
    t->start();
    t->join();
}