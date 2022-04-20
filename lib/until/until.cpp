//
// Created by wyatt on 2022/3/28.
//

#include "until.h"

pid_t wyatt::until::GetThreadId() {
    return syscall(SYS_gettid);
}

void wyatt::until::BackTrace(std::vector<std::string> &vec, int size, int skip)         {
    void **array = (void **) malloc(sizeof(void *) * size);
    size_t s = backtrace(array, size);
    char** strings = backtrace_symbols(array, s);
    for (int i = skip; i < s; ++i) {
        vec.emplace_back(strings[i]);
    }
    free(array);
    free(strings);
}

std::string wyatt::until::BackTrace(int size, int skip, const std::string &prefix)        {
    std::vector<std::string> vec;
    BackTrace(vec, size, skip);
    std::stringstream ss;
    for (auto &v: vec) {
        ss << prefix << v << std::endl;
    }
    return ss.str();
}

uint64_t wyatt::until::Now()         {
    return std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::system_clock::now()).time_since_epoch().count();
}

uint64_t wyatt::until::TimeAfter(uint64_t delay)         {
//            cout << (chrono::system_clock::now() + chrono::microseconds(delay)).time_since_epoch().count() << endl;
//            cout << (chrono::system_clock::now()).time_since_epoch().count() << endl;
    return std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::system_clock::now() + std::chrono::microseconds(delay * 1000)).time_since_epoch().count();
}

void wyatt::until::createTimer(int timerfd, uint64_t delay) {
    struct itimerspec howlong{};
    bzero(&howlong, sizeof howlong);
    howlong.it_value.tv_sec = delay / 1000;
    howlong.it_value.tv_nsec = (delay % 1000) * 1000 * 1000;
    ::timerfd_settime(timerfd, 0, &howlong, nullptr);
}