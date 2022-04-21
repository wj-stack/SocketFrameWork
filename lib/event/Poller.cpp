//
// Created by wyatt on 2022/4/19.
//

#include "Poller.h"

Poller::Poller() {
    epfd = epoll_create(10); // 在新版这个参数已经没意义了
}

int Poller::poll(int TimeOutMs, Poller::ChannelList *activeChannelList) {
    if ((int) pollfds.size() == 0)return 0;
    int nfd = epoll_wait(epfd, pollfds.data(), (int) pollfds.size(), TimeOutMs);
    if (nfd > 0) {
        for (int i = 0; i < nfd; ++i) {
            auto &channel = channelList[pollfds[i].data.fd];
            channel->setEvent(pollfds[i].events); // 为当前channel设置事件
            (*activeChannelList)[pollfds[i].data.fd] = channel;  // 处理事件
        }
    } else if (nfd == 0) {
        WYATT_LOG_ROOT_DEBUG() << "nothing to do";
    } else {
        WYATT_LOG_ROOT_DEBUG() << "poll();" << nfd;
    }

    return nfd;

}

void Poller::updateChannel(Channel *channel) {
    if (channel->getIndex() < 0) // index 是 chhannel在 pollfds 的位置
    {
        // a new one
        struct epoll_event ev{};
        ev.data.fd = channel->getFd();
        ev.events = channel->getEvent();
        channelList[ev.data.fd] = channel;
        epoll_ctl(epfd, EPOLL_CTL_ADD, ev.data.fd, &ev);

        if (!freefds.empty()) {
            channel->setIndex((int) freefds.front());
            pollfds[(int) freefds.front()] = ev;
            freefds.pop_front();

        } else {
            channel->setIndex((int) pollfds.size());
            pollfds.emplace_back(ev);
        }

    } else {
        // update
        auto ev = pollfds[channel->getIndex()];
        ev.data.fd = channel->getFd();
        ev.events = channel->getEvent();

        channelList[ev.data.fd] = channel;
        if (ev.events == 0) {
            // 没有事件感兴趣，则屏蔽
            ev.data.fd = ev.data.fd - 1;
        } else {
            epoll_ctl(epfd, EPOLL_CTL_MOD, ev.data.fd, &ev);

        }
    }

    // 更新指定fd的感兴趣的状态
//        channel->getFd();

}

void Poller::removeChannel(Channel *channel) {
    WYATT_LOG_ROOT_DEBUG() << "Poller::removeChannel :" << channel->getIndex();
    if (channel->getIndex() >= 0) {
        auto ev = pollfds[channel->getIndex()];
        WYATT_LOG_ROOT_DEBUG() << "Poller::removeChannel :" << channel->getIndex();
        WYATT_LOG_ROOT_DEBUG() << "EPOLL_CTL_DEL :" << ev.data.fd;
        freefds.push_back(channel->getIndex());
        epoll_ctl(epfd, EPOLL_CTL_DEL, ev.data.fd, &ev);
        channel->setIndex(-1);
        WYATT_LOG_ROOT_DEBUG() << "EPOLL_CTL_DEL :" << ev.data.fd;
    }
}
