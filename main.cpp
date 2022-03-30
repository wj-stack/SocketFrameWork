#include <iostream>
#include <memory>
#include "lib/config/config.h"
#include "lib/log/loggerManager.h"
#include "lib/thread/thread.h"
#include <mutex>
#include <thread>



std::mutex _mutex;
std::fstream fs;

void fun1()
{
//    fs << i << '\n';
    auto p =  wyatt::Thread::getThis();;
    if(p)
    {
        std::lock_guard<std::mutex> lockGuard{_mutex};
        WYATT_LOG_ROOT_DEBUG() << p->getId();
    }
//    WYATT_LOG_ROOT_DEBUG() << wyatt::Thread::getThisThread();
//    WYATT_LOG_ROOT_DEBUG() << wyatt::Thread::getThisThread()->getId();
}

int main() {


//    std::cout << getpid() << std::endl;
    fs.open("1.txt", ios::app);
    vector<wyatt::Thread::ptr> th;
    for (int i = 0; i < 100; ++i) {
        th.emplace_back(std::make_shared<wyatt::Thread>(fun1, "name_" + to_string(i)));
    }


    sleep(5);

    for (int i = 0; i < 100; ++i) {
        th[i]->join();
    }
    fs.close();
    return 0;
}
