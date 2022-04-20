//
// Created by wyatt on 2022/4/19.
//

#ifndef SOCKETFRAMEWORK_POLLER_H
#define SOCKETFRAMEWORK_POLLER_H
#include "../log/loggerManager.h"
#include <vector>
#include <map>
#include <list>
#include "Channel.h"
class Poller{
public:
    using ChannelList = std::map<int,Channel*>; // 一个fd对应着一个channel

    Poller();

    int poll(int TimeOutMs,ChannelList* activeChannelList);

    void updateChannel(Channel* channel);

    void removeChannel(Channel* channel);

private:
    ChannelList channelList;
    std::vector<struct epoll_event> pollfds; // fd的集合
    std::list<int> freefds;
    int epfd = -1;

};


#endif //SOCKETFRAMEWORK_POLLER_H
