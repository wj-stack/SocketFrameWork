//
// Created by wyatt on 2022/3/28.
//

#ifndef LOG_FORMATTER_H
#define LOG_FORMATTER_H

#include <memory>
#include "event.h"
#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include <functional>
#include <string>
#include "level.h"

//XX(m, MessageFormatItem),           //m:消息
//XX(p, LevelFormatItem),             //p:日志级别
//XX(r, ElapseFormatItem),            //r:累计毫秒数
//XX(c, NameFormatItem),              //c:日志名称
//XX(t, ThreadIdFormatItem),          //t:线程id
//XX(n, NewLineFormatItem),           //n:换行
//XX(d, DateTimeFormatItem),          //d:时间
//XX(f, FilenameFormatItem),          //f:文件名
//XX(l, LineFormatItem),              //l:行号
//XX(T, TabFormatItem),               //T:Tab
//XX(F, FiberIdFormatItem),           //F:协程id
//XX(N, ThreadNameFormatItem),        //N:线程名称

class FormatItem
{
public:
    using ptr = std::shared_ptr<FormatItem>;
    virtual  void format(std::ostream& os,Event::ptr event) = 0;
    virtual ~FormatItem() = default;
};


#define XX(item) \
class item##FormatItem: public FormatItem\
{\
public:          \
        explicit item##FormatItem(const std::string& str){} \
void format(std::ostream &os, Event::ptr event) override; \
};
XX(Message)
XX(Elapse)
XX(Name)
XX(ThreadId)
XX(File)
XX(Line)
XX(FiberId)
XX(ThreadName)
#undef XX

class TimeFormatItem: public FormatItem
{
    std::string dateFormat;
public:
    explicit TimeFormatItem(const std::string& dateFormat = "%Y-%m-%d %H:%M:%S");
    void format(std::ostream &os, Event::ptr event) override;
};

class LevelFormatItem: public FormatItem
{
public:
    explicit LevelFormatItem(const std::string& str){}
    void format(std::ostream &os, Event::ptr event) override;
};

class TabFormatItem: public FormatItem
{
public:
    explicit TabFormatItem(const std::string& str){}
    void format(std::ostream &os, Event::ptr event) override;
};

class NewLineFormatItem: public FormatItem
{
public:
    explicit NewLineFormatItem(const std::string& str){}
    void format(std::ostream &os, Event::ptr event) override;
};

class StringFormatItem: public FormatItem
{
private:
    std::string str;
public:
    explicit StringFormatItem(std::string  str) : str(std::move(str)){}
    void format(std::ostream &os, Event::ptr event) override;
};


class Formatter {
private:
    std::vector<FormatItem::ptr> m_items;
public:
    using ptr = std::shared_ptr<Formatter>;
    explicit Formatter(const std::string& format = "%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n");
    void format(std::ostream& os,const Event::ptr& event);

};


#endif //LOG_FORMATTER_H
