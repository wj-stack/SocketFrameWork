//
// Created by wyatt on 2022/3/27.
//

#ifndef LOG_EVENT_H
#define LOG_EVENT_H

#include <string>
#include <memory>
#include <utility>
#include <chrono>
#include "level.h"
#include <iostream>
class Event {

private:
    std::string Message;
    Level::level level;
    uint32_t elapse{};
    std::string name;
    int32_t threadId;
    uint64_t time{};
    std::string file;
    int32_t line;
    int32_t fiberId;
    std::string threadName;
    static std::chrono::time_point<std::chrono::system_clock, std::chrono::microseconds> startTime;
    std::chrono::time_point<std::chrono::system_clock, std::chrono::microseconds> nowTime;

public:
    using ptr = std::shared_ptr<Event>;

    Event(std::string msg, Level::level level, std::string name, int32_t threadId,
          std::string file, int32_t line, int32_t fiberId, std::string threadName) : Message(std::move(msg)),
                                                                                     level(level),
                                                                                     name(std::move(name)),
                                                                                     threadId(threadId),
                                                                                     file(std::move(file)), line(line),
                                                                                     fiberId(fiberId),
                                                                                     threadName(std::move(threadName)) {

        using namespace std;
        // 获取操作系统当前时间点（精确到微秒）
        nowTime = chrono::time_point_cast<chrono::microseconds>(chrono::system_clock::now());
        elapse = nowTime.time_since_epoch().count() - startTime.time_since_epoch().count();
        time = nowTime.time_since_epoch().count() / 1000000;

    }

public:
    const std::string &getMessage() const {
        return Message;
    }

    Level::level getLevel() const {
        return level;
    }

    uint32_t getElapse() const {
        return elapse;
    }

    const std::string &getName() const {
        return name;
    }

    int32_t getThreadId() const {
        return threadId;
    }

    uint64_t getTime() const {
        return time;
    }

    const std::string &getFile() const {
        return file;
    }

    int32_t getLine() const {
        return line;
    }

    int32_t getFiberId() const {
        return fiberId;
    }

    const std::string &getThreadName() const {
        return threadName;
    }


};


#endif //LOG_EVENT_H
