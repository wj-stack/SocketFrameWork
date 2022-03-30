//
// Created by wyatt on 2022/3/28.
//

#include "loggerManager.h"


namespace wyatt
{

    LoggerManager::ptr LoggerManager::loggerManager;

    LoggerManager::LoggerManager()         {
        Logger::ptr logger = make_shared<Logger>("root", Level::DEBUG, make_shared<Formatter>());
        logger->addAppender(shared_ptr<Appender>(new StdoutAppender()));
        loggers["root"] = logger;
    }

    LoggerManager::ptr LoggerManager::getInstance()         {
        if(!loggerManager)
        {
            loggerManager = shared_ptr<LoggerManager>(new LoggerManager());
            loggerManager->init();
        }
        return loggerManager;
    }

    Logger::ptr LoggerManager::getRootLogger() {
        return loggers["root"];
    }

    Logger::ptr LoggerManager::getLogger(const string &name) {
        return loggers[name];
    }

    void LoggerManager::addLogger(const string &name, Level::level level, const string &format,
                                  const vector<Appender::ptr> &appenders)        {
        Logger::ptr logger = make_shared<Logger>(name, level, make_shared<Formatter>(format));
        for(auto& appender:appenders)
        {
            logger->addAppender(appender);
        }
        loggers[name] = logger;
    }

    void LoggerManager::init() {
        config.lookUp("logs", vector<LogConfigVar>());
        config.LoadYamlConfig("./logger.yaml");
        auto logsPtr = config.lookUp<vector<LogConfigVar>>("logs");
        vector<LogConfigVar> logs = logsPtr->getValue();
        for (auto &v: logs) {
            auto level = level_map.find(v.getLevel());
            if (level != level_map.end())
            {
                auto appenderConfig = v.getAppenderConfig();
                vector<Appender::ptr> appenders;
                for (auto &appender: appenderConfig)
                {
                    if(appender.getType() == "StdoutAppender")
                    {
                        appenders.push_back(shared_ptr<Appender>(new StdoutAppender()));
                    }else if(appender.getType() == "FileAppender")
                    {
                        appenders.push_back(shared_ptr<Appender>(new FileAppender(appender.getFile().c_str())));
                    }
                }
                addLogger(v.getName(), level->second, v.getFormatter(), appenders);
            }
        }


    }


}