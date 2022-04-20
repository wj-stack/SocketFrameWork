//
// Created by wyatt on 2022/4/20.
//

#ifndef SOCKETFRAMEWORK_TIMERMANAGER_H
#define SOCKETFRAMEWORK_TIMERMANAGER_H

#include <set>
#include "Timer.h"
#include "Channel.h"
#include <functional>
#include <memory>
class EventLoop;
class TimerManager {
public:

    struct TimerEvent {
        bool operator()(const shared_ptr<Timer> timer1,const shared_ptr<Timer> timer2);
    };
    std::set<std::shared_ptr<Timer>,TimerEvent> timer_set;

    explicit TimerManager(EventLoop* p);
    ~TimerManager();
    void addTimer( const std::function<void()>& cb, uint64_t delay,bool recycle);
    void cancel(int id);

private:
    static void TimeOut(TimerManager* p);
    EventLoop* loop;
    int timerfd;
    Channel channel;
};


#endif //SOCKETFRAMEWORK_TIMERMANAGER_H
