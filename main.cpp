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
#include "assert.h"

#include "lib/event/EventLoop.h"
#include "lib/event/Channel.h"

#include <sys/timerfd.h>



EventLoop e;

void timeOut()
{
    WYATT_LOG_ROOT_DEBUG() << "TIME OUT" ;
    e.quit();
}

void fun1()
{
    WYATT_LOG_ROOT_DEBUG() << "fun1" <<endl;

    int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);

    Channel channel(&e,timerfd);
    channel.setReadCallBack(&timeOut);
    channel.enableReading();

    struct itimerspec howlong;
    bzero(&howlong, sizeof howlong);
    howlong.it_value.tv_sec = 5;
    ::timerfd_settime(timerfd, 0, &howlong, nullptr);

    e.loop();
    close(timerfd);
}

int main() {
//
    wyatt::Thread t(fun1,"fun1");

    t.join();



    return 0;
}
