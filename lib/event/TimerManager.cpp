//
// Created by wyatt on 2022/4/20.
//
#include "EventLoop.h"
#include "TimerManager.h"

#include <memory>

bool TimerManager::TimerEvent::operator()(const shared_ptr<Timer> timer1,const shared_ptr<Timer> timer2)
{
    if (timer1->getTimeStamp() != timer2->getTimeStamp())return timer1->getTimeStamp() < timer2->getTimeStamp();
    return timer1 < timer2;
}

void TimerManager::TimeOut(TimerManager *p)
{

    while (!p->timer_set.empty() && (*p->timer_set.begin())->getTimeStamp() <= wyatt::until::Now()) {
        auto& timer = (*p->timer_set.begin());
        p->loop->runInLoop([&](){
            timer->run();
            p->readTimerfd();
            WYATT_LOG_ROOT_DEBUG() << "定时器事件！";
        });
        if(timer->isRecycle())
        {
            timer->reset();
            p->timer_set.insert(timer);
        }
        p->timer_set.erase(p->timer_set.begin());
    }
}

TimerManager::TimerManager(EventLoop* p) : loop(p) , timerfd(::timerfd_create(CLOCK_MONOTONIC,
                                                                              TFD_NONBLOCK | TFD_CLOEXEC)) , channel(loop,timerfd){
    channel.enableReading();
    channel.setReadCallBack([&]() {
        TimeOut(this);
    });

}
TimerManager::~TimerManager() {
    ::close(timerfd);
}

void TimerManager::addTimer( const std::function<void()>& cb, uint64_t delay,bool recycle){
    timer_set.insert(std::make_shared<Timer>(cb, delay, recycle));
    loop->runInLoop(std::bind(&wyatt::until::createTimer,timerfd,delay));
}
void TimerManager::cancel(int id){

}

void TimerManager::readTimerfd()    {
    uint64_t howmany;
    ssize_t n = ::read(timerfd, &howmany, sizeof howmany);
    if (n != sizeof howmany)
    {
        WYATT_LOG_ROOT_DEBUG() << "TimerQueue::handleRead() reads " << n << " bytes instead of 8";
    }
}
