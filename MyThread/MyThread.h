#pragma once

#include <thread>

class MyThread
{
public:
    MyThread();
    ~MyThread();

    virtual void run() = 0;

    void Start();

    void Stop();
public:
    std::thread* mythread = nullptr;
    int stopFlag = 0;
};