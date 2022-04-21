//
// Created by wyatt on 2022/4/21.
//

#include "Acceptor.h"

Acceptor::Acceptor(EventLoop *_loop, InetAddress *listen) : loop(_loop), listenAddr(listen),
                                                            sockfd(Socket::createSocket()), channel(loop,sockfd) {

}




void Acceptor::listen() {
    Socket::bind(sockfd, *listenAddr);
    Socket::listen(sockfd, 1024);
    channel.enableReading();
    channel.setReadCallBack([&](){
        WYATT_LOG_ROOT_DEBUG() << "have client";
        handleRead();
    });

}

bool Acceptor::listening() const {
    return isListening;
}

void Acceptor::setNewConnectCallBack(const Acceptor::callback &cb) {
    m_cb = cb;
}

void Acceptor::handleRead() {

    assert(loop->isInLoopThread());
    InetAddress inetAddress{};
    int connfd = Socket::accept(sockfd,inetAddress);
    if (connfd > 0)
    {
        if (m_cb){
            m_cb(connfd,inetAddress);
        }
    }else
    {
        ::close(connfd);
    }

}
