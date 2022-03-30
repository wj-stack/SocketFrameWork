//)
// Created by wyatt on 2022/3/28.
//

#ifndef LOG_LOGGERMANAGER_H
#define LOG_LOGGERMANAGER_H

#include "EventWarp.h"
#include "logger.h"
#include <map>
#include "../config/config.h"

//#define WYATT_LOG_DEBUG(logger) EventWarp(logger, Level::DEBUG,__FILE__,__LINE__).getSs()
#define WYATT_LOG_ROOT_DEBUG() EventWarp(LoggerManager::getInstance()->getRootLogger(), Level::DEBUG,__FILE__,__LINE__).getSs()
#define WYATT_LOG_DEBUG(logger) EventWarp(LoggerManager::getInstance()->getLogger(logger), Level::DEBUG,__FILE__,__LINE__).getSs()




class AppenderConfigVar {
private:
    std::string type;
    std::string file;

public:
    const string &getType() const {
        return type;
    }

    void setType(const string &type) {
        AppenderConfigVar::type = type;
    }

    const string &getFile() const {
        return file;
    }

    void setFile(const string &file) {
        AppenderConfigVar::file = file;
    }

public:
    AppenderConfigVar(const string &type, const string &file) : type(type), file(file) {}
};

class LogConfigVar {
private:
    std::string name;
    std::string level;
    std::string formatter;
    std::vector<AppenderConfigVar> appenderConfig;
public:
    const string &getName() const {
        return name;
    }

    void setName(const string &name) {
        LogConfigVar::name = name;
    }

    const string &getLevel() const {
        return level;
    }

    void setLevel(const string &info) {
        LogConfigVar::level = info;
    }

    const string &getFormatter() const {
        return formatter;
    }

    void setFormatter(const string &formatter) {
        LogConfigVar::formatter = formatter;
    }

    const vector<AppenderConfigVar> &getAppenderConfig() const {
        return appenderConfig;
    }

    void setAppenderConfig(const vector<AppenderConfigVar> &appenderConfig) {
        LogConfigVar::appenderConfig = appenderConfig;
    }

public:
    LogConfigVar() = default;

};

template<>
class LexicalCast<std::string, AppenderConfigVar> {
public:
    AppenderConfigVar operator()(const std::string &str) {
        YAML::Node node = YAML::Load(str);
        AppenderConfigVar appenderConfigVar(node["type"].IsNull() ? "" : node["type"].Scalar(),
                                            node["file"].IsNull() ? "" : node["file"].Scalar());
        return appenderConfigVar; // C++ 11会自动优化
    }
};

template<>
class LexicalCast<AppenderConfigVar, std::string> {
public:
    std::string operator()(const AppenderConfigVar &appenderConfigVar) {
        YAML::Node node;
        node["type"] = appenderConfigVar.getType();
        if (!appenderConfigVar.getFile().empty())
            node["file"] = appenderConfigVar.getFile();
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

template<>
class LexicalCast<std::string, LogConfigVar> {
public:
    LogConfigVar operator()(const std::string &str) {
        YAML::Node node = YAML::Load(str);
        LogConfigVar logConfigVar;
        logConfigVar.setName(node["name"].Scalar());
        logConfigVar.setLevel(node["level"].Scalar());
        logConfigVar.setFormatter(node["formatter"].Scalar());
        std::stringstream ss;
        ss << node["appender"];
        logConfigVar.setAppenderConfig(LexicalCast<std::string, vector<AppenderConfigVar>>()(ss.str()));
        return logConfigVar; // C++ 11会自动优化
    }
};

template<>
class LexicalCast<LogConfigVar, std::string> {
public:
    std::string operator()(const LogConfigVar &logConfigVar) {
        YAML::Node node;
        node["name"] = logConfigVar.getName();
        node["level"] = logConfigVar.getLevel();
        node["formatter"] = logConfigVar.getFormatter();
        node["appender"] = LexicalCast<vector<AppenderConfigVar>, std::string>()(logConfigVar.getAppenderConfig());
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};


class LoggerManager {
private:
    map<string, Logger::ptr> loggers;
    Config config;
    LoggerManager()
    {
        Logger::ptr logger = make_shared<Logger>("root", Level::DEBUG, make_shared<Formatter>());
        logger->addAppender(shared_ptr<Appender>(new StdoutAppender()));
        loggers["root"] = logger;
    }
public:
    using ptr = shared_ptr<LoggerManager>;

    static LoggerManager::ptr loggerManager;

    static LoggerManager::ptr getInstance()
    {
        if(!loggerManager)
        {
            loggerManager = shared_ptr<LoggerManager>(new LoggerManager());
            loggerManager->init();
        }
        return loggerManager;
    }

    Logger::ptr getRootLogger(){
        return loggers["root"];
    }

    Logger::ptr getLogger(const string& name){
        return loggers[name];
    }

    void addLogger(const std::string& name,Level::level level,const std::string& format,const std::vector<Appender::ptr>& appenders)
    {
        Logger::ptr logger = make_shared<Logger>(name, level, make_shared<Formatter>(format));
        for(auto& appender:appenders)
        {
            logger->addAppender(appender);
        }
        loggers[name] = logger;
    }


    const std::map<std::string , Level::level> level_map{
            {"debug",Level::DEBUG},
            {"info",Level::INFO},
            {"warn",Level::WARN},
            {"error",Level::ERROR},
            {"fatal",Level::FATAL},
    };

    void init() {
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
};

LoggerManager::ptr LoggerManager::loggerManager;

#endif //LOG_LOGGERMANAGER_H
