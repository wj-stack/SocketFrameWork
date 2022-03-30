//)
// Created by wyatt on 2022/3/28.
//

#ifndef LOG_LOGGERMANAGER_H
#define LOG_LOGGERMANAGER_H

#include "eventwarp.h"
#include "logger.h"
#include <map>
#include "../config/config.h"

#define WYATT_LOG_ROOT_DEBUG() wyatt::EventWarp(wyatt::LoggerManager::getInstance()->getRootLogger(), wyatt::Level::DEBUG,__FILE__,__LINE__).getSs()
#define WYATT_LOG_DEBUG(logger) wyatt::EventWarp(wyatt::LoggerManager::getInstance()->getLogger(logger), wyatt::Level::DEBUG,__FILE__,__LINE__).getSs()

namespace wyatt
{

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
        LoggerManager();
    public:
        using ptr = shared_ptr<LoggerManager>;

        static LoggerManager::ptr loggerManager;

        static LoggerManager::ptr getInstance();

        Logger::ptr getRootLogger();

        Logger::ptr getLogger(const string& name);

        void addLogger(const std::string& name,Level::level level,const std::string& format,const std::vector<Appender::ptr>& appenders);

        const std::map<std::string , Level::level> level_map{
                {"debug",Level::DEBUG},
                {"info",Level::INFO},
                {"warn",Level::WARN},
                {"error",Level::ERROR},
                {"fatal",Level::FATAL},
        };

        void init();
    };

    LoggerManager::ptr LoggerManager::loggerManager;

}
#endif //LOG_LOGGERMANAGER_H
