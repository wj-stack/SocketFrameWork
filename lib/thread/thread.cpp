//
// Created by wyatt on 2022/3/30.
//

#include "thread.h"

namespace wyatt
{
    thread_local Thread* Thread::this_thread;
    thread_local std::string Thread::this_name;

    Thread::Thread(std::function<void()> cb, const string &name) : name(name), cb(std::move(cb)) {
        int rt = pthread_create(&threadId, nullptr, &Thread::run, this);
        if (rt) {
            WYATT_LOG_ROOT_DEBUG() << "pthread_create thread fail, rt=" << rt
                                   << " name=" << name;
            throw std::logic_error("pthread_create error");
        }
        semaphore.wait(); // 阻塞住，等待线程初始化完成
    }

    void Thread::join()
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

    Thread::~Thread() {
        if(threadId) {
            pthread_detach(threadId);
        }
    }

    void *Thread::run(void *args)  {
        Thread* thread = (Thread*)args;
        this_thread = thread;
        this_name = thread->name;
        pthread_setname_np(pthread_self(), thread->name.substr(0, 15).c_str());
        thread->id = until::GetThreadId();
        std::function<void()> tmp;
        tmp.swap(thread->cb);
        thread->semaphore.notify(); // 线程初始化完成
        tmp();
        return 0;
    }

    void Thread::setThisName(const string &v) {
        if(v.empty()) {
            return;
        }
        WYATT_LOG_ROOT_DEBUG() << "this_thread:" << this_thread ;
        if(this_thread) {
            this_thread->name = name;
        }
        this_name = v;
        WYATT_LOG_ROOT_DEBUG() << "this name:" << this_name;
    }
}
