//
// Created by wyatt on 2022/4/21.
//

#ifndef SOCKETFRAMEWORK_TCPSERVER_H
#define SOCKETFRAMEWORK_TCPSERVER_H
#include "EventLoop.h"
#include "Acceptor.h"
#include "InetAddress.h"
#include "TcpConnection.h"
#include <functional>
#include <map>
class TcpServer {
public:
    typedef std::function<void(const TcpConnection::ptr&)> ConnectCallBack;
    typedef std::function<void(const TcpConnection::ptr&,char*,size_t)> ReadCallBack;
    typedef std::function<void(const TcpConnection::ptr&)> CloseCallBack;
    typedef std::function<void(const TcpConnection::ptr&)> ErrorCallBack;
    TcpServer(EventLoop *l, InetAddress *inetAddress);
    void start();
    void setConnectCallBack(const ConnectCallBack& callBack);
    void setReadCallBack(const ReadCallBack& callBack);
    void setCloseCallBack(const CloseCallBack& callBack);
    void setErrorCallBack(const ErrorCallBack& callBack);

private:
    void ReadHandel(const TcpConnection::ptr&);
    void CloseHandel(const TcpConnection::ptr&);
    void ErrorHandel(const TcpConnection::ptr&);
    static int s_id;
    std::map<int,TcpConnection::ptr> Clients;
    EventLoop* loop;
    Acceptor acceptor;
    ConnectCallBack connectCallBack;
    ReadCallBack readCallBack;
    CloseCallBack closeCallBack;
    ErrorCallBack errorCallBack;
};


#endif //SOCKETFRAMEWORK_TCPSERVER_H
