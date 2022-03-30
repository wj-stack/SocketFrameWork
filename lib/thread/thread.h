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

namespace wyatt {

    class Thread {
    private:
        static thread_local Thread* this_thread;
        static thread_local std::string this_name;

        std::string name;
        pthread_t threadId {0};
        pid_t id{};
        std::function<void()> cb;

    public:
        using ptr = std::shared_ptr<Thread>;


        void setThisName(const std::string& v) {
            if(v.empty()) {
                return;
            }
            if(this_thread) {
                this_thread->name = name;
            }
            this_name = v;
        }

        Thread(std::function<void()> cb, const std::string &name) : name(name), cb(std::move(cb)) {
            int rt = pthread_create(&threadId, nullptr, &Thread::run, this);
//            WYATT_LOG_ROOT_DEBUG() << "threadId:" << threadId;
            if (rt) {
                WYATT_LOG_ROOT_DEBUG() << "pthread_create thread fail, rt=" << rt
                                       << " name=" << name;
                throw std::logic_error("pthread_create error");
            }
        }

        static Thread* getThis(){ return this_thread; }
        static const std::string& getThisName() { return this_name; }

        void join()
        {
            if(threadId) {
                int rt = pthread_join(threadId, nullptr);
                if(rt) {
                    WYATT_LOG_ROOT_DEBUG() << "pthread_join thread fail, rt=" << rt
                                              << " name=" << name;
                    throw std::logic_error("pthread_join error");
                }
                threadId = 0;
            }
        }

        pid_t getId() const{ return id; }
        pthread_t getThreadId() const{ return threadId; }

        ~Thread(){
            if(threadId) {
                pthread_detach(threadId);
            }
        }

    private:
        static void *run(void *args) {
            Thread* thread = (Thread*)args;
            this_thread = thread;
            this_name = thread->name;
            thread->id = until::GetThreadId();
            std::function<void()> tmp;
            tmp.swap(thread->cb);
            tmp();
            return 0;
        }
    };
}

#endif //SOCKETFRAMEWORK_THREAD_H
