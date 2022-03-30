//
// Created by wyatt on 2022/3/28.
//

#ifndef LOG_LOGGER_H
#define LOG_LOGGER_H
#include <memory>
#include <set>
#include <utility>
#include "appender.h"
#include "formatter.h"

namespace wyatt{
    class Logger {
    private:
        set<Appender::ptr> appenders;
        string name;
        Level::level level;
        Formatter::ptr formatter;

    public:
        Logger(string name, Level::level level, Formatter::ptr formatter) : name(std::move(name)), level(level),
                                                                            formatter(std::move(formatter)) {}

    public:
        using ptr = std::shared_ptr<Logger>;


        void addAppender(Appender::ptr appender);

        void eraseAppender(Appender::ptr appender);

        void log(const Event::ptr& event);

        const string& getName() const { return name; }
    };


}
#endif //LOG_LOGGER_H
