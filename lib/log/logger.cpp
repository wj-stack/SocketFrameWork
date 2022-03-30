//
// Created by wyatt on 2022/3/28.
//

#include "logger.h"

namespace wyatt{
    void Logger::log(const Event::ptr &event) {
        if (event->getLevel() >= level) {
            for (auto &appender: appenders) {
                formatter->format(appender->getOstream(), event);
            }
        }
    }

    void Logger::eraseAppender(Appender::ptr appender)    {
        appenders.erase(appender);
    }

    void Logger::addAppender(Appender::ptr appender) {
        appenders.insert(std::move(appender));
    }

}