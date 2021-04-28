#include "MyThread.h"

MyThread::MyThread()
{

}
MyThread::~MyThread()
{

}

void MyThread::Start()
{
    if (mythread == nullptr) {
        mythread = new std::thread(&MyThread::run, this);  
        // run can be dynamicaly binded
    }
}

void MyThread::Stop()
{
    stopFlag = 1;
}