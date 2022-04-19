//
// Created by wyatt on 2022/3/31.
//

#include "mutex.h"

namespace wyatt
{


    Semaphore::Semaphore(uint32_t count) {
        if(sem_init(&m_semaphore, 0, count))
        {
            throw std::logic_error("semaphore init error");
        }
    }

    Semaphore::~Semaphore() {
        sem_destroy(&m_semaphore);
    }

    void Semaphore::wait() {
        if (sem_wait(&m_semaphore)) {
            throw std::logic_error("semaphore wait error");
        }
    }

    void Semaphore::notify() {
        if(sem_post(&m_semaphore)){
            throw std::logic_error("semaphore notify error");
        }
    }

}