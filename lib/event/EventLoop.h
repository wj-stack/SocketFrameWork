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

#include "Channel.h"
#include "Poller.h"
class EventLoop {
private:
    const int TimeOutMs = -1;
    static thread_local EventLoop* t_loopInThread;
    atomic_bool looping {false};
    atomic_bool quit_ {false};
    std::shared_ptr<Poller> poller{new Poller()};
public:
    EventLoop()
    {
        if(t_loopInThread)
        {
            WYATT_LOG_ROOT_DEBUG() << "Another EventLoop" << t_loopInThread << "existed in this thread"
                                   << wyatt::Thread::getThis();
        }else
        {
            t_loopInThread = this;
        }
        WYATT_LOG_ROOT_DEBUG() << " creat eventLoop";
    }
    ~EventLoop()
    {
        WYATT_LOG_ROOT_DEBUG() << " del eventLoop";
        t_loopInThread = nullptr;
    }

    void loop()
    {
        assert(!looping);
        looping = true;
        Poller::ChannelList channelList;
        while (!quit_)
        {
            channelList.clear();
            poller->poll(TimeOutMs,&channelList);
            for(auto& v:channelList)
            {
                auto& channel = v.second;
                channel->handleEvent();
            }
        }
        WYATT_LOG_ROOT_DEBUG() << "loop is stopping";
        t_loopInThread = nullptr;
        looping = false;
    }

    void updateChannel(Channel* channel){
        // 更新感兴趣的事件
        poller->updateChannel(channel);

    }

    void quit()
    {
        quit_ = true;
    }

};


#endif //SOCKETFRAMEWORK_EVENTLOOP_H
