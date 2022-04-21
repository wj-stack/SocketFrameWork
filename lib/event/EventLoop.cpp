//
// Created by wyatt on 2022/4/19.
//

#include "EventLoop.h"


thread_local EventLoop *EventLoop::t_loopInThread;

int EventLoop::createEventfd() {
    int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if (evtfd < 0) {
        WYATT_LOG_ROOT_DEBUG() << "Failed in eventfd";
        abort();
    }
    return evtfd;
}

void EventLoop::wakeup() {
    WYATT_LOG_ROOT_DEBUG() << "WAKEUp";
    uint64_t one = 1;
    ssize_t n = ::write(wakefd, &one, sizeof one);
    if (n != sizeof one) {
        WYATT_LOG_ROOT_DEBUG() << "EventLoop::wakeup() writes " << n << " bytes instead of 8";
    }
}

void EventLoop::handleRead() {
    WYATT_LOG_ROOT_DEBUG() << "handleRead";
    uint64_t one = 1;
    ssize_t n = ::read(wakefd, &one, sizeof one);
    if (n != sizeof one) {
        WYATT_LOG_ROOT_DEBUG() << "EventLoop::handleRead() reads " << n << " bytes instead of 8";
    }
}

EventLoop::EventLoop() : wakefd(createEventfd()), weakChannel(this, wakefd) ,timerManager(new TimerManager(this)) {
    if (t_loopInThread) {
        WYATT_LOG_ROOT_DEBUG() << "Another EventLoop" << t_loopInThread << "existed in this thread"
                               << wyatt::Thread::getThis();
    } else {
        weakChannel.enableReading();
        weakChannel.setReadCallBack([&]() {
            handleRead();
        });
        WYATT_LOG_ROOT_DEBUG() << " weakChannel index: " << weakChannel.getIndex();
        t_loopInThread = this;
        threadId = wyatt::until::GetThreadId();
    }
    WYATT_LOG_ROOT_DEBUG() << " creat eventLoop" << this << " bind thread: " << wyatt::Thread::getThis();
}

EventLoop::~EventLoop() {
    WYATT_LOG_ROOT_DEBUG() << " del eventLoop";
    t_loopInThread = nullptr;
    ::close(wakefd);
}

void EventLoop::removeChannel(Channel *channel) {
    poller->removeChannel(channel);
}

void EventLoop::loop() {
    assert(!looping);
    looping = true;
    Poller::ChannelList channelList;
    while (!quit_) {
        channelList.clear();
        poller->poll(TimeOutMs, &channelList);
        for (auto &v: channelList) {
            auto &channel = v.second;
            channel->handleEvent();
//            WYATT_LOG_ROOT_DEBUG() << "event fd:"  << v.first;
        }
        WYATT_LOG_ROOT_DEBUG() << "event " << channelList.size();
        CallBackQueue();
    }
    WYATT_LOG_ROOT_DEBUG() << "loop is stopping";
    t_loopInThread = nullptr;
    looping = false;
}

void EventLoop::CallBackQueue() {
    std::lock_guard<std::mutex> lockGuard{mutex_};
    for (auto &i: callBackQueue) {
        i();
        WYATT_LOG_ROOT_DEBUG() << "call back" << &i;
    }
    callBackQueue.clear();
}

void EventLoop::updateChannel(Channel *channel) {
    // 更新感兴趣的事件
    poller->updateChannel(channel);
//        WYATT_LOG_ROOT_DEBUG() << "EventLoop updateChannel" ;
}

void EventLoop::quit() {
    quit_ = true;
    if (!isInLoopThread() && !callBackQueue.empty())wakeup();
}

void EventLoop::runInLoop(const EventLoop::CB &cb) {
    if (isInLoopThread()) {
        cb();
    } else {
        WYATT_LOG_ROOT_DEBUG() << "add callBackQueue";
        callBackQueue.emplace_back(cb);
        wakeup();
    }
}

bool EventLoop::isInLoopThread() const {
    return getCurThreadId() == threadId;
}

pid_t EventLoop::getCurThreadId() { return wyatt::until::GetThreadId(); }

void EventLoop::runAfter(const EventLoop::CB &cb, uint64_t delay) {
    timerManager->addTimer(cb, delay, false);
}

void EventLoop::runAt(const EventLoop::CB &cb) {
    timerManager->addTimer(cb, 1, false);
}


