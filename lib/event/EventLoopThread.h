//
// Created by wyatt on 2022/4/20.
//

#ifndef SOCKETFRAMEWORK_EVENTLOOPTHREAD_H
#define SOCKETFRAMEWORK_EVENTLOOPTHREAD_H

#include "EventLoop.h"
#include "../thread/thread.h"
#include <mutex>
#include <condition_variable>
#include "../thread/mutex.h"

class EventLoopThread {
private:
    std::condition_variable cv;
    EventLoop* loop;
    std::mutex mutex_;
    wyatt::Semaphore semaphore;
    void ThreadFunc(){
        EventLoop eventLoop;
        {
            loop = &eventLoop;
            semaphore.notify();
        }
        WYATT_LOG_ROOT_DEBUG() << "loop:  " << loop;
        loop->loop();
    }
public:
    EventLoop* startLoop(const std::string& name = "EventLoop"){
        wyatt::Thread t([&](){
            ThreadFunc();
        },name);
        semaphore.wait();
        return loop;
    }
};


#endif //SOCKETFRAMEWORK_EVENTLOOPTHREAD_H
