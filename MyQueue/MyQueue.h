#pragma once

#include <queue>
#include <mutex>

template<typename T>
class MyQueue
{
public:
    MyQueue()
    {

    }
    
    ~MyQueue() {
        std::lock_guard<std::mutex> lck(mut);
		while (size() > 0) {
			T * t = myQueue.front();
            delete t;
			myQueue.pop();
		}
    }

    int pop(T** t) {
        std::lock_guard<std::mutex> lck(mut);

        if (size() > 0) {
            *t = myQueue.front();
            myQueue.pop();
            return 0;
        }

        return -1;
    }

    int push(T* t) {
        std::lock_guard<std::mutex> lck(mut);

		myQueue.push(t);

		return 0;
    }

    int size() {
        return myQueue.size();
    }

private:
    std::queue<T *> myQueue;
    std::mutex mut;
};