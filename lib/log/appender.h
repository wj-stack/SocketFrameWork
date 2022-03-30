//
// Created by wyatt on 2022/3/28.
//

#ifndef LOG_APPENDER_H
#define LOG_APPENDER_H

#include <memory>
#include <iostream>
#include <fstream>

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
        std::fstream fs;
    public:
        explicit FileAppender(const char* filepath){
            fs.open(filepath, ios_base::app);
        }
        ostream & getOstream() override{
            return fs;
        }
        ~FileAppender() override{
            fs.flush();
            fs.close();
        }
    };
}

#endif //LOG_APPENDER_H
