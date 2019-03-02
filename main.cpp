#include <iostream>
#include"base/Thread.h"
#include "base/LogFile.h"
using namespace std;
void* dosomework1()
{
    cout << "gan" << endl;
}

int main()
{
   LogFile* lf = new LogFile("hshsh");
   lf->append("xixiix", sizeof("xixiix"));
   lf->append("wori", sizeof("wori"));
   lf->flush();
}