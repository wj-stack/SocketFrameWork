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

void Channel::enableWriting()  {event_ |= EVENT_WRITE ; update();}

void Channel::handleEvent(){
    if (revent & EVENT_ERR)
    {
        m_errorCb();
    }

    if (revent & EVENT_READ)
    {
        m_readCb();
    }
    if(revent & EVENT_WRITE)
    {
        m_writeCb();
    }
}

void Channel::setEvent(int revt) { revent = revt; }

int Channel::getEvent()  { return event_; }

int Channel::getFd()   { return fd; }