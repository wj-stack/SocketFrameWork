//
// Created by wyatt on 2022/3/31.
//

#ifndef SOCKETFRAMEWORK_MUTEX_H
#define SOCKETFRAMEWORK_MUTEX_H

#include <boost/noncopyable.hpp>
#include <memory>
#include <functional>
#include <pthread.h>
#include <mutex>
#include <semaphore.h>
#include <error.h>

namespace wyatt {


    class Semaphore : public boost::noncopyable {
    private:
        sem_t m_semaphore;
    public:
        explicit Semaphore(uint32_t count = 0);

        ~Semaphore();

        void wait();

        void notify();
    };


    template<class T>
    class ScopedLockImpl : boost::noncopyable{
    public:
        ScopedLockImpl(T& mutex) : m_mutex(mutex)
        {
            lock();
        }
        ~ScopedLockImpl(){
            unlock();
        }

        void lock()
        {
            if(!isLock)
            {
                m_mutex.lock();
                isLock = true;
            }
        }

        void unlock()
        {
            if(isLock)
            {
                m_mutex.unlock();
                isLock = false;
            }
        }

    private:
        T& m_mutex;
        bool isLock;
    };

    template<class T>
    class ReadScopedLockImpl : boost::noncopyable{
    public:
        ReadScopedLockImpl(T& mutex) : m_mutex(mutex)
        {
            lock();
        }
        ~ReadScopedLockImpl(){
            unlock();
        }

        void lock()
        {
            if(!isLock)
            {
                m_mutex.rdlock();
                isLock = true;
            }
        }

        void unlock()
        {
            if(isLock)
            {
                m_mutex.unlock();
                isLock = false;
            }
        }

    private:
        T& m_mutex;
        bool isLock;
    };

    template<class T>
    class WriteScopedLockImpl : boost::noncopyable{
    public:
        WriteScopedLockImpl(T& mutex) : m_mutex(mutex)
        {
            lock();
        }
        ~WriteScopedLockImpl(){
            unlock();
        }

        void lock()
        {
            if(!isLock)
            {
                m_mutex.wrlock();
                isLock = true;
            }
        }

        void unlock()
        {
            if(isLock)
            {
                m_mutex.unlock();
                isLock = false;
            }
        }

    private:
        T& m_mutex;
        bool isLock;
    };


    /**
 * @brief ???????????????
 */
    class RWMutex : boost::noncopyable {
    public:

        /// ????????????
        typedef ReadScopedLockImpl<RWMutex> ReadLock;

        /// ????????????
        typedef WriteScopedLockImpl<RWMutex> WriteLock;

        /**
         * @brief ????????????
         */
        RWMutex() {
            pthread_rwlock_init(&m_lock, nullptr);
        }

        /**
         * @brief ????????????
         */
        ~RWMutex() {
            pthread_rwlock_destroy(&m_lock);
        }

        /**
         * @brief ?????????
         */
        void rdlock() {
            pthread_rwlock_rdlock(&m_lock);
        }

        /**
         * @brief ?????????
         */
        void wrlock() {
            pthread_rwlock_wrlock(&m_lock);
        }

        /**
         * @brief ??????
         */
        void unlock() {
            pthread_rwlock_unlock(&m_lock);
        }
    private:
        /// ?????????
        pthread_rwlock_t m_lock;
    };



/**
 * @brief ?????????
 */
    class Mutex : boost::noncopyable {
    public:
        /// ?????????
        typedef ScopedLockImpl<Mutex> Lock;

        /**
         * @brief ????????????
         */
        Mutex() {
            pthread_mutex_init(&m_mutex, nullptr);
        }

        /**
         * @brief ????????????
         */
        ~Mutex() {
            pthread_mutex_destroy(&m_mutex);
        }

        /**
         * @brief ??????
         */
        void lock() {
            pthread_mutex_lock(&m_mutex);
        }

        /**
         * @brief ??????
         */
        void unlock() {
            pthread_mutex_unlock(&m_mutex);
        }
    private:
        /// mutex
        pthread_mutex_t m_mutex;
    };


}


#endif //SOCKETFRAMEWORK_MUTEX_H
