//
// Created by wyatt on 2022/4/20.
//

#include "EventLoopThread.h"

EventLoop *EventLoopThread::startLoop(const string &name) {
    wyatt::Thread t([&]() {
        ThreadFunc();
    }, name);
    semaphore.wait();
    return loop;
}

void EventLoopThread::ThreadFunc() {
    EventLoop eventLoop;
    {
        loop = &eventLoop;
        semaphore.notify();
    }
    WYATT_LOG_ROOT_DEBUG() << "loop:  " << loop;
    loop->loop();
}