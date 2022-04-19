//
// Created by wyatt on 2022/3/30.
//

#ifndef SOCKETFRAMEWORK_THREAD_H
#define SOCKETFRAMEWORK_THREAD_H

#include <memory>
#include <functional>
#include <pthread.h>
#include <string>
#include <utility>
#include "../log/loggerManager.h"
#include "../until//until.h"
#include <boost/noncopyable.hpp>
#include "mutex.h"

namespace wyatt {

class Thread :public boost::noncopyable {

    private:
        static thread_local Thread *this_thread;
        static thread_local std::string this_name;

        std::string name;
        pthread_t threadId{0};
        pid_t id{};
        std::function<void()> cb;
        Semaphore semaphore;

    public:
        using ptr = std::shared_ptr<Thread>;


        void setThisName(const std::string &v);

        Thread(std::function<void()> cb, const std::string &name);

        static Thread *getThis() { return this_thread; }

        static const std::string &getThisName() { return this_name; }

        void join();

        pid_t getId() const { return id; }

        pthread_t getThreadId() const { return threadId; }

        ~Thread();

    private:
        static void *run(void *args);
    };
}

#endif //SOCKETFRAMEWORK_THREAD_H
