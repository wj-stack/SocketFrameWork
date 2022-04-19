//
// Created by wyatt on 2022/3/28.
//

#include "logger.h"

namespace wyatt{
    void Logger::log(const Event::ptr &event) {
        wyatt::Mutex::Lock lock(mutex);
        if (event->getLevel() >= level) {
            for (auto &appender: appenders) {
                formatter->format(appender->getOstream(), event);
            }
        }
    }

    void Logger::eraseAppender(Appender::ptr appender){
        wyatt::Mutex::Lock lock(mutex);
        appenders.erase(appender);
    }

    void Logger::addAppender(Appender::ptr appender) {
        wyatt::Mutex::Lock lock(mutex);
        appenders.insert(std::move(appender));
    }

}