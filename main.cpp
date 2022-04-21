#include <iostream>
#include <memory>
#include "lib/config/config.h"
#include "lib/log/loggerManager.h"
#include "lib/thread/thread.h"
#include "lib/until//until.h"
#include "lib/thread/mutex.h"
#include <mutex>
#include <thread>
#include <execinfo.h>
#include <memory>

#include "lib/event/EventLoop.h"
#include "lib/event/Channel.h"
#include "lib/event/Timer.h"
#include "lib/event/TimerManager.h"
#include "lib/event/EventLoopThread.h"
#include <sys/timerfd.h>

#include "lib/event/InetAddress.h"
#include "lib/event/TcpServer.h"
#include "lib/event/Buffer.h"

void newConnect(const TcpConnection::ptr &conn) {

    WYATT_LOG_ROOT_DEBUG() << "new Connect" << " " << conn->getFd();
}


void ReadEvent(const TcpConnection::ptr &conn, Buffer &buf, size_t t) {
    WYATT_LOG_ROOT_DEBUG() << "read";

    WYATT_LOG_ROOT_DEBUG() << buf.begin() + 8 << " " << (int) t;
}


void CloseEvent(const TcpConnection::ptr &conn) {
    WYATT_LOG_ROOT_DEBUG() << "close";
}

int main() {
    InetAddress inetAddress(8888, false, false);
    EventLoop eventLoop;
    TcpServer tcpServer(&eventLoop, &inetAddress);
    tcpServer.setConnectCallBack(newConnect);
    tcpServer.setReadCallBack(ReadEvent);
    tcpServer.setCloseCallBack(CloseEvent);
    tcpServer.setErrorCallBack(CloseEvent);
    tcpServer.start();
//    Acceptor acceptor(&eventLoop,&inetAddress);
//    acceptor.setNewConnectCallBack(newConnect);
//    acceptor.listen();

    eventLoop.loop();
    return 0;
}
