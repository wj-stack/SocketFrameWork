//
// Created by wyatt on 2022/3/28.
//

#ifndef LOG_APPENDER_H
#define LOG_APPENDER_H

#include <memory>
#include <iostream>
#include <fstream>
#include <map>
using namespace std;

namespace wyatt
{

    class Appender {
    public:
        using ptr = std::shared_ptr<Appender>;
        virtual ostream& getOstream() = 0;
        virtual ~Appender()= default;

    };

    class StdoutAppender  : public Appender
    {
    public:
        ostream & getOstream() override{
            return cout;
        }
    };

    class FileAppender  : public Appender
    {
    private:
        static std::map<std::string,std::fstream> mp;
        std::string m_path;
    public:
        explicit FileAppender(const char* filepath) : m_path(filepath){
            if (mp.find(filepath) == mp.end()) {
                mp[filepath].open(filepath, ios_base::app);
            }
        }
        ostream & getOstream() override{
            return mp[m_path];
        }
        ~FileAppender() override{
            mp[m_path].flush();
            mp[m_path].close();
        }
    };


}

#endif //LOG_APPENDER_H
