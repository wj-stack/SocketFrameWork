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
class until {
public:


    static pid_t GetThreadId() {
        return syscall(SYS_gettid);
    }
};


#endif //LOG_UNTIL_H
