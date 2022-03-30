//
// Created by wyatt on 2022/3/28.
//

#include "config.h"


void Config::loadYamlConfig(const std::string &prefix, const YAML::Node &node,
                            std::list<std::pair<std::string, YAML::Node>> &output) {

    output.emplace_back(prefix, node);
    if (node.IsMap()) {
        for (auto it = node.begin();
             it != node.end(); ++it) {
            loadYamlConfig(prefix.empty() ? it->first.Scalar()
                                          : prefix + "." + it->first.Scalar(), it->second, output);
        }
    }
}

void Config::LoadYamlConfig(const std::string &path) {
    std::list<std::pair<std::string, YAML::Node> > output;
    YAML::Node root = YAML::LoadFile(path);
    Config::loadYamlConfig("", root, output);
    for (auto &item: output) {
        auto ptr = lookUpBase(item.first);
        if (ptr) {
            if (item.second.IsScalar()) {
                ptr->fromString(item.second.Scalar());
            } else {
                std::stringstream ss;
                ss << item.second;
                ptr->fromString(ss.str());
            }
        }
    }
}

ConfigVarBase::ptr Config::lookUpBase(const std::string &name) {
    return config[name];
}

ConfigVarBase::ConfigVarBase(const std::string &name, const std::string &description) : name(name),
                                                                                        description(description) {

}
