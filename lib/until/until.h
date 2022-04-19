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

namespace wyatt
{
    class until {
    public:


        static pid_t GetThreadId() {
            return syscall(SYS_gettid);
        }

        static void BackTrace(std::vector<std::string>& vec,int size,int skip = 0)
        {
            void **array = (void **) malloc(sizeof(void *) * size);
            size_t s = backtrace(array, size);
            char** strings = backtrace_symbols(array, s);
            for (int i = skip; i < s; ++i) {
                vec.emplace_back(strings[i]);
            }
            free(array);
            free(strings);
        }

        static std::string BackTrace(int size, int skip, const std::string &prefix)
        {
            std::vector<std::string> vec;
            BackTrace(vec, size, skip);
            std::stringstream ss;
            for (auto &v: vec) {
                ss << prefix << v << endl;
            }
            return ss.str();
        }


    };

}


#endif //LOG_UNTIL_H
