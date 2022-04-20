//
// Created by wyatt on 2022/4/19.
//

#ifndef SOCKETFRAMEWORK_EVENTLOOP_H
#define SOCKETFRAMEWORK_EVENTLOOP_H

#include "../log/loggerManager.h"
#include <sys/select.h>
#include <sys/epoll.h>
#include <sys/poll.h>
#include <atomic>
#include <functional>
#include <vector>
#include <sys/timerfd.h>
#include <sys/eventfd.h>
#include "Channel.h"
#include "Poller.h"
#include "TimerManager.h"

class EventLoop {
private:
    const int TimeOutMs = -1;
    static thread_local EventLoop *t_loopInThread;
    atomic_bool looping{false};
    atomic_bool quit_{false};
    pid_t threadId;

    std::shared_ptr<Poller> poller{new Poller()};
    typedef std::function<void()> CB;
    vector<CB> callBackQueue;
    std::mutex mutex_;

    int wakefd;
    Channel weakChannel;
    std::unique_ptr<TimerManager> timerManager;


    int createEventfd();
    void wakeup(); // 唤醒
    void handleRead(); // 处理唤醒，防止一直唤醒
    void CallBackQueue(); // 消费者，处理添加进来的函数

public:



    static pid_t getCurThreadId();


    EventLoop();

    ~EventLoop();

    void removeChannel(Channel *channel);

    void loop();

    void updateChannel(Channel *channel);

    void quit();

    bool isInLoopThread() const;

    void runInLoop(const CB &cb);

    void runAt(const CB& cb);
    void runAfter(const CB& cb ,uint64_t delay);
};


#endif //SOCKETFRAMEWORK_EVENTLOOP_H
