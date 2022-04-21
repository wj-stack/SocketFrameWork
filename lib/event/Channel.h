//
// Created by wyatt on 2022/4/19.
//

#ifndef SOCKETFRAMEWORK_CHANNEL_H
#define SOCKETFRAMEWORK_CHANNEL_H

#include <sys/epoll.h>
#include <functional>

class EventLoop;
class Channel{
private:
    using CB = std::function<void()>;
    const int EVENT_NONE = 0;
    const int EVENT_READ = EPOLLIN | EPOLLPRI;
    const int EVENT_WRITE = EPOLLOUT;
    const int EVENT_ERR = EPOLLERR;
    const int EVENT_CLOSE = EPOLLRDHUP;
    int fd;
    int index = -1; // 默认在pollfds中的位置
    int event_ = 0; //当前Channel感兴趣的事件
    int revent = 0; //当前Channel真实的事件
    EventLoop* loop;
    void update();

public:

    Channel(EventLoop* loop_,int fd_) : loop(loop_) , fd(fd_) {}
    void setIndex(int idx){ index = idx; }
    int getIndex() const { return index; }
    void setReadCallBack(const CB &mReadCb);
    void setWriteCallBack(const CB &mWriteCb);
    void setErrorCallBack(const CB &mErrorCb);
    void setCloseCallBack(const CB &mCloseCb);

    void enableReading();
    void enableWriting();
    void disableAll();
    void remove();
    void setEvent(int revt);// 设置当前事件
    int getEvent(); // 获取感兴趣的事件

    int getFd();

    void handleEvent();


private:
    CB m_writeCb;
    CB m_errorCb;
    CB m_readCb;
    CB m_closeCb;

};


#endif //SOCKETFRAMEWORK_CHANNEL_H
