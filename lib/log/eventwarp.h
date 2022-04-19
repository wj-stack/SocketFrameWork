//
// Created by wyatt on 2022/3/28.
//

#ifndef LOG_EVENTWARP_H
#define LOG_EVENTWARP_H

#include "event.h"
#include "logger.h"
#include <sstream>
#include <thread>
#include "../until/until.h"
#include "../thread/mutex.h"
#include "../thread/thread.h"

namespace wyatt{
    class EventWarp {

    public:
        EventWarp(const Logger::ptr &logger, Level::level level, const string &file, uint32_t line) : logger(
                logger), name(logger->getName()), level(level), file(file), line(line) {}
        std::ostream& getSs();
        ~EventWarp();

    private:
        std::stringstream ss;
        Level::level level;
        Logger::ptr logger;
        std::string name;
        string file;
        int32_t line;

    };

}

#endif //LOG_EVENTWARP_H
