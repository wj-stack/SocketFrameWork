//
// Created by wyatt on 2022/4/19.
//

#ifndef SOCKETFRAMEWORK_POLLER_H
#define SOCKETFRAMEWORK_POLLER_H
#include "../log/loggerManager.h"
#include <vector>
#include <map>
#include "Channel.h"
class Poller{
public:
    using ChannelList = std::map<int,Channel*>; // 一个fd对应着一个channel

    Poller()
    {
        epfd = epoll_create(10); // 在新版这个参数已经没意义了
    }

    int poll(int TimeOutMs,ChannelList* activeChannelList)
    {
        if((int)pollfds.size() == 0)return 0;
        int nfd  = epoll_wait(epfd, pollfds.data(), (int)pollfds.size(), TimeOutMs);
        if (nfd > 0)
        {
            for (int i = 0; i < nfd; ++i) {
                auto& channel = channelList[pollfds[i].data.fd];
                channel->setEvent(pollfds[i].events); // 为当前channel设置事件
                (*activeChannelList)[pollfds[i].data.fd] = channel;  // 处理事件
            }
        }else if(nfd == 0)
        {
            WYATT_LOG_ROOT_DEBUG() << "nothing to do";
        }else
        {
            WYATT_LOG_ROOT_DEBUG() << "poll();" << nfd;
        }

        return nfd;

    }

    void updateChannel(Channel* channel)
    {
        if (channel->getIndex() < 0) // index 是 chhannel在 pollfds 的位置
        {
            // a new one
            struct epoll_event ev{};
            ev.data.fd = channel->getFd();
            ev.events = channel->getEvent();
            channelList[ev.data.fd] = channel;
            epoll_ctl(epfd, EPOLL_CTL_ADD, ev.data.fd, &ev);
            pollfds.emplace_back(ev);
        }else
        {
            // update
            auto ev = pollfds[channel->getIndex()];
            ev.data.fd = channel->getFd();
            ev.events = channel->getEvent();
            channelList[ev.data.fd] = channel;
            if(ev.events == 0)
            {
                // 没有事件感兴趣，则屏蔽
                ev.data.fd = ev.data.fd - 1;
            }
        }

        // 更新指定fd的感兴趣的状态
//        channel->getFd();

    }
private:
    ChannelList channelList;
    std::vector<struct epoll_event> pollfds; // fd的集合
    int epfd = -1;

};


#endif //SOCKETFRAMEWORK_POLLER_H
