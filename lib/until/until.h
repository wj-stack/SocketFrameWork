//
// Created by wyatt on 2022/3/28.
//

#ifndef LOG_UNTIL_H
#define LOG_UNTIL_H

#include <execinfo.h>
#include <sys/time.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <syscall.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <execinfo.h>
#include <sys/timerfd.h>
#include <vector>
#include <string>
#include <sstream>
#include <chrono>
namespace wyatt
{
    class until {
    public:


        static pid_t GetThreadId();

        static void BackTrace(std::vector<std::string>& vec,int size,int skip = 0);

        static std::string BackTrace(int size, int skip, const std::string &prefix);
        static uint64_t Now();
        static uint64_t TimeAfter(uint64_t delay) ;// 纳米级别

        static void createTimer(int timerfd, uint64_t delay);


    };

}


#endif //LOG_UNTIL_H
