//
// Created by wyatt on 2022/4/21.
//

#ifndef SOCKETFRAMEWORK_TCPCONNECTION_H
#define SOCKETFRAMEWORK_TCPCONNECTION_H

#include "Channel.h"
#include "EventLoop.h"
class TcpConnection : std::enable_shared_from_this<TcpConnection>{
public:
    using ptr = shared_ptr<TcpConnection>;
    enum STATE{CONNECTING,CONNECTED,ERROR,CLOSE};
    TcpConnection(EventLoop* loop_,int fd_,int id_);
    int getState() const { return state; }
    void setState(STATE s) { state = s; }
    Channel* getChannel() { return &channel; }
    int getFd() { return fd; }
private:
    int id;
    EventLoop* loop;
    int fd;
    Channel channel;
    STATE state = CONNECTING;
};


#endif //SOCKETFRAMEWORK_TCPCONNECTION_H
