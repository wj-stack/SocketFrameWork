//
// Created by wyatt on 2022/4/19.
//

#include "Channel.h"
#include "EventLoop.h"

void Channel::setReadCallBack(const Channel::CB &mReadCb) {
    m_readCb = mReadCb;
}

void Channel::setWriteCallBack(const Channel::CB &mWriteCb) {
    m_writeCb = mWriteCb;
}

void Channel::setErrorCallBack(const Channel::CB &mErrorCb) {
    m_errorCb = mErrorCb;
}

void Channel::update(){
    loop->updateChannel(this);
}

void Channel::enableReading() {event_ |= EVENT_READ ; update();}
void Channel::disableAll() {event_ = EVENT_NONE ; update();}

void Channel::enableWriting()  {event_ |= EVENT_WRITE ; update();}

void Channel::handleEvent(){
    if (revent & EVENT_ERR)
    {
        WYATT_LOG_ROOT_DEBUG() << "channel err";
        if(m_errorCb)m_errorCb();
        return ;
    }
    if ((revent & EVENT_CLOSE)) {
        WYATT_LOG_ROOT_DEBUG() << "channel close";
        if(m_closeCb)m_closeCb();
        return ;
    }

    if (revent & EVENT_READ)
    {
        if(m_readCb)m_readCb();
    }
    if(revent & EVENT_WRITE)
    {
        if(m_writeCb)m_writeCb();
    }
}

void Channel::setEvent(int revt) { revent = revt; }

int Channel::getEvent()  { return event_; }

int Channel::getFd()   { return fd; }

void Channel::remove() {
    loop->removeChannel(this);
}

void Channel::setCloseCallBack(const Channel::CB &mCloseCb) {
    m_closeCb = mCloseCb;
}
