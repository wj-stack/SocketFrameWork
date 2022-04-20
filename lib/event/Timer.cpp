//
// Created by wyatt on 2022/4/20.
//

#include "Timer.h"
#include <iostream>
int Timer::sid = 0;


Timer::Timer(const std::function<void()> &cb, uint64_t delay, bool recycle) {
    id = ++sid;
    this->delay = delay;
    this->cb = cb;
    this->recycle = recycle;
    this->reset();
}

void Timer::reset() {
    timeStamp = wyatt::until::TimeAfter(delay);
}

void Timer::run() {
    cb();
}
