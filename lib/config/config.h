//
// Created by wyatt on 2022/3/28.
//

#ifndef LOG_CONFIGVARBASE_H
#define LOG_CONFIGVARBASE_H

#include <memory>
#include <string>
#include <boost/lexical_cast.hpp>
#include <utility>
#include <yaml-cpp/yaml.h>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>


namespace wyatt
{
    class ConfigVarBase {
    public:
        using ptr = std::shared_ptr<ConfigVarBase>;
        std::string name;
        std::string description;



        ConfigVarBase(const std::string &name, const std::string &description);

        virtual std::string toString() = 0;

        virtual bool fromString(const std::string &) = 0; // 初始化
        virtual ~ConfigVarBase() = default;
    };

    template<class From, class To>
    class LexicalCast {
    public:
        To operator()(const From &str) {
            return boost::lexical_cast<To>(str);
        }
    };

    template<class T>
    class LexicalCast<std::string, std::vector<T> > {
    public:

        std::vector<T> operator()(const std::string &str) {
            YAML::Node node = YAML::Load(str);
            std::stringstream ss;
            typename std::vector<T> vec;
            for (int i = 0; i < node.size(); ++i) {
                ss.str("");
                ss << node[i];
                vec.push_back(LexicalCast<std::string, T>()(ss.str()));
            }
            return vec;
        }
    };

    template<class T>
    class LexicalCast<std::vector<T>, std::string> {
    public:
        std::string operator()(const std::vector<T> &vec) {
            YAML::Node node;
            for (auto &v: vec) {
                node.push_back(YAML::Load(LexicalCast<T, std::string>()(v)));
            }
            std::stringstream ss;
            ss << node;
            return ss.str();
        }
    };


    template<class T>
    class LexicalCast<std::string, std::list<T> > {
    public:
        std::list<T> operator()(const std::string &str) {
            YAML::Node node = YAML::Load(str);
            std::stringstream ss;
            typename std::list<T> vec;
            for (int i = 0; i < node.size(); ++i) {
                ss.str("");
                ss << node[i];
                vec.push_back(LexicalCast<std::string, T>()(ss.str()));
            }
            return vec;
        }
    };

    template<class T>
    class LexicalCast<std::list<T>, std::string> {
    public:
        std::string operator()(const std::list<T> &vec) {
            YAML::Node node;
            for (auto &v: vec) {
                node.push_back(YAML::Load(LexicalCast<T, std::string>()(v)));
            }
            std::stringstream ss;
            ss << node;
            return ss.str();
        }
    };

    template<class T>
    class LexicalCast<std::string, std::set<T> > {
    public:
        std::set<T> operator()(const std::string &str) {
            YAML::Node node = YAML::Load(str);
            std::stringstream ss;
            typename std::set<T> vec;
            for (int i = 0; i < node.size(); ++i) {
                ss.str("");
                ss << node[i];
                vec.insert(LexicalCast<std::string, T>()(ss.str()));
            }
            return vec;
        }
    };

    template<class T>
    class LexicalCast<std::set<T>, std::string> {
    public:
        std::string operator()(const std::set<T> &vec) {
            YAML::Node node;
            for (auto &v: vec) {
                node.push_back(YAML::Load(LexicalCast<T, std::string>()(v)));
            }
            std::stringstream ss;
            ss << node;
            return ss.str();
        }
    };

    template<class T>
    class LexicalCast<std::string, std::unordered_set<T> > {
    public:
        std::unordered_set<T> operator()(const std::string &str) {
            YAML::Node node = YAML::Load(str);
            std::stringstream ss;
            typename std::unordered_set<T> vec;
            for (int i = 0; i < node.size(); ++i) {
                ss.str("");
                ss << node[i];
                vec.insert(LexicalCast<std::string, T>()(ss.str()));
            }
            return vec;
        }
    };

    template<class T>
    class LexicalCast<std::unordered_set<T>, std::string> {
    public:
        std::string operator()(const std::unordered_set<T> &vec) {
            YAML::Node node;
            for (auto &v: vec) {
                node.push_back(YAML::Load(LexicalCast<T, std::string>()(v)));
            }
            std::stringstream ss;
            ss << node;
            return ss.str();
        }
    };

    template<class T>
    class LexicalCast<std::string, std::map<std::string, T> > {
    public:
        std::map<std::string, T> operator()(const std::string &str) {
            YAML::Node node = YAML::Load(str);
            std::stringstream ss;
            typename std::map<std::string, T> mp;
            for (auto it = node.begin(); it != node.end(); ++it) {
                ss.str("");
                ss << it->second;
                mp.insert({it->first.Scalar(), LexicalCast<std::string, T>()(ss.str())});
            }
            return mp;
        }
    };

    template<class T>
    class LexicalCast<std::map<std::string, T>, std::string> {
    public:
        std::string operator()(const std::map<std::string, T> &vec) {
            YAML::Node node;
            for (auto &v: vec) {
                node[v.first] = YAML::Load(LexicalCast<T, std::string>()(v.second));
            }
            std::stringstream ss;
            ss << node;
            return ss.str();
        }
    };

    template<class T>
    class LexicalCast<std::string, std::unordered_map<std::string, T> > {
    public:
        std::unordered_map<std::string, T> operator()(const std::string &str) {
            YAML::Node node = YAML::Load(str);
            std::stringstream ss;
            typename std::unordered_map<std::string, T> mp;
            for (auto it = node.begin(); it != node.end(); ++it) {
                ss.str("");
                ss << it->second;
                mp.insert({it->first.Scalar(), LexicalCast<std::string, T>()(ss.str())});
            }
            return mp;
        }
    };

    template<class T>
    class LexicalCast<std::unordered_map<std::string, T>, std::string> {
    public:
        std::string operator()(const std::unordered_map<std::string, T> &vec) {
            YAML::Node node;
            for (auto &v: vec) {
                node[v.first] = YAML::Load(LexicalCast<T, std::string>()(v.second));
            }
            std::stringstream ss;
            ss << node;
            return ss.str();
        }
    };


    template<class T, class FormStr = LexicalCast<std::string, T>, class ToStr = LexicalCast<T, std::string>>
    class ConfigVar : public ConfigVarBase {
    private:
        T m_val;
    public:
        using ptr = std::shared_ptr<ConfigVar<T>>;

        ConfigVar(const std::string &name, const T &default_value, const std::string &description = "")
                : ConfigVarBase(name, description), m_val(default_value) {
        }

        std::string toString() override {
            try {

                return ToStr()(m_val);

            } catch (std::exception &e) {
                return "";
            }
        }

        bool fromString(const std::string &value) override {
            try {
                setValue(FormStr()(value));
                return true;
            } catch (std::exception &e) {
                return false;
            }
        }

        T getValue() const {
            return m_val;
        }

        void setValue(const T &val) {
            m_val = val;
        }
    };

    class Config {
    private:
        std::map<std::string, ConfigVarBase::ptr> config;

    public:

        ConfigVarBase::ptr lookUpBase(const std::string &name);

        template<class T>
        typename ConfigVar<T>::ptr lookUp(const std::string &name) {
            return std::dynamic_pointer_cast<ConfigVar<T> >(config[name]);
        }


        template<class T>
        typename ConfigVar<T>::ptr lookUp(const std::string &name, const T &value, const std::string &description = "") {
            if (config.find(name) == config.end()) {
                config[name] = std::shared_ptr<ConfigVarBase>(new ConfigVar<T>(name, value, description));
            }
            return std::dynamic_pointer_cast<ConfigVar<T> >(config[name]);
        }

        static void loadYamlConfig(const std::string &prefix, const YAML::Node &node,
                                   std::list<std::pair<std::string, YAML::Node> > &output);


        void LoadYamlConfig(const std::string &path);
    };

}


#endif //LOG_CONFIGVARBASE_H
