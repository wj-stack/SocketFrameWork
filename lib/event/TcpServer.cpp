//
// Created by wyatt on 2022/4/21.
//

#include "TcpServer.h"

#include <memory>


int TcpServer::s_id = 0;

TcpServer::TcpServer(EventLoop *l, InetAddress* inetAddress) : loop(l) , acceptor(loop,inetAddress){

}

void ConnectCallBack()
{

}


void TcpServer::start() {
    acceptor.setNewConnectCallBack([&](int fd,const InetAddress& inetAddress){ // 客户新连接

        auto ptr = std::make_shared<TcpConnection>(loop, fd,++s_id);
        Channel& channel = *ptr->getChannel();
        channel.enableReading();
        channel.setReadCallBack(std::bind(&TcpServer::ReadHandel,this, ptr)); // 先转发给readHandel然后转发给客户
        channel.setCloseCallBack(std::bind(&TcpServer::CloseHandel,this, ptr)); // 先转发给readHandel然后转发给客户
        channel.setErrorCallBack(std::bind(&TcpServer::ErrorHandel,this, ptr)); // 先转发给readHandel然后转发给客户
        Clients[s_id] = ptr;

        connectCallBack(ptr); // 调用Connect回调函数

    });
    acceptor.listen();

}

void TcpServer::setConnectCallBack(const TcpServer::ConnectCallBack & cb) {
    connectCallBack = cb;
}

void TcpServer::setReadCallBack(const TcpServer::ReadCallBack &callBack) {
    readCallBack = callBack;
}

void TcpServer::ReadHandel(const TcpConnection::ptr& ptr) {
    char* buf = new char[1024];
    size_t n = 0;
     n = ::read(ptr->getFd(),buf,1024);
    if(n > 0)
    {
        WYATT_LOG_ROOT_DEBUG() << "n:" << n;
        WYATT_LOG_ROOT_DEBUG() << "fd:" << ptr->getFd();
        if(readCallBack)readCallBack(ptr,buf,n); // 执行客户回调
    }
    delete[] buf;
}

void TcpServer::setCloseCallBack(const TcpServer::CloseCallBack &callBack) {
    closeCallBack = callBack;
}

void TcpServer::CloseHandel(const TcpConnection::ptr & ptr) {
    ptr->setState(TcpConnection::CLOSE);
    if(closeCallBack)closeCallBack(ptr);
    ::close(ptr->getFd());
}

void TcpServer::setErrorCallBack(const TcpServer::CloseCallBack &callBack) {
    errorCallBack = callBack;
}

void TcpServer::ErrorHandel(const TcpConnection::ptr & ptr) {
    ptr->setState(TcpConnection::ERROR);
    if(errorCallBack)errorCallBack(ptr);
    ::close(ptr->getFd());
}
