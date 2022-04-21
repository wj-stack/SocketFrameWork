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
#include "lib/event/Acceptor.h"

void newConnect(int fd,const InetAddress& conn)
{
    WYATT_LOG_ROOT_DEBUG() << "new client:" << fd;
    WYATT_LOG_ROOT_DEBUG() << "new client:" << conn.toIp() << " " << conn.toPort();
    const char* a = "hello world\n";
    ::write(fd,a,strlen(a));
    Socket::close(fd);
}

int main() {
    InetAddress inetAddress(8888, false, false);
    EventLoop eventLoop;
    Acceptor acceptor(&eventLoop,&inetAddress);
    acceptor.setNewConnectCallBack(newConnect);
    acceptor.listen();
    eventLoop.loop();
    return 0;
}
