//
// Created by wyatt on 2022/3/28.
//

#include "formatter.h"



#define XX(item) \
void item##FormatItem::format(std::ostream &os, Event::ptr event) {   \
    os << event->get##item();             \
    }

XX(Message)
XX(Elapse)
XX(Name)
XX(ThreadId)
XX(File)
XX(Line)
XX(FiberId)
XX(ThreadName)
#undef XX


void TimeFormatItem::format(std::ostream &os, Event::ptr event) {
    //// 这里要用对应的格式
    struct tm tm{};
    time_t time = event->getTime();
    localtime_r(&time, &tm);
    char buf[64];
    strftime(buf, sizeof(buf), dateFormat.c_str(), &tm);
    os << buf;
}

TimeFormatItem::TimeFormatItem(const std::string &dateFormat):dateFormat(dateFormat) {
    if (dateFormat.empty()) {
        this->dateFormat = "%Y-%m-%d %H:%M:%S";
    }
}

void LevelFormatItem::format(std::ostream &os, Event::ptr event)     {
    os << Level::toString(event->getLevel());
}

void TabFormatItem::format(std::ostream &os, Event::ptr event)     {
    os << '\t';
}

void NewLineFormatItem::format(std::ostream &os, Event::ptr event)     {
    os << '\n';
}

void StringFormatItem::format(std::ostream &os, Event::ptr event)    {
    os << str;
}

Formatter::Formatter(const std::string &format) {
    // 格式化
    std::vector<std::tuple<std::string, std::string, int> > vec;
    std::string nstr;
    for (int i = 0; i < format.size(); ++i) {
        if(format[i] != '%') {
            nstr.append(1, format[i]);
            continue;
        }

        // %%
        if((i + 1) < format.size()) {
            if(format[i + 1] == '%') {
                nstr.append(1, '%');
                continue;
            }
        }

        size_t n = i + 1;
        int fmt_status = 0;
        size_t fmt_begin = 0;

        std::string str;
        std::string fmt;
        while(n < format.size()) {
            if(!fmt_status && (!isalpha(format[n]) && format[n] != '{'
                               && format[n] != '}')) {
                str = format.substr(i + 1, n - i - 1);
                break;
            }
            if(fmt_status == 0) {
                if(format[n] == '{') {
                    str = format.substr(i + 1, n - i - 1);
                    //std::cout << "*" << str << std::endl;
                    fmt_status = 1; //解析格式
                    fmt_begin = n;
                    ++n;
                    continue;
                }
            } else if(fmt_status == 1) {
                if(format[n] == '}') {
                    fmt = format.substr(fmt_begin + 1, n - fmt_begin - 1);
                    //std::cout << "#" << fmt << std::endl;
                    fmt_status = 0;
                    ++n;
                    break;
                }
            }
            ++n;
            if(n == format.size()) {
                if(str.empty()) {
                    str = format.substr(i + 1);
                }
            }
        }

        if(fmt_status == 0) {
            if(!nstr.empty()) {
                vec.emplace_back(nstr, std::string(), 0);
                nstr.clear();
            }

            vec.emplace_back(str, fmt, 1);

            i = n - 1;
        } else if(fmt_status == 1) {
            std::cout << "pattern parse error: " << format << " - " << format.substr(i) << std::endl;
//                m_error = true;
            vec.emplace_back("<<pattern_error>>", fmt, 0);

        }
    }

    if(!nstr.empty()) {
        vec.emplace_back(nstr, "", 0);
    }

//        for (auto& i:vec) {
//            cout << std::get<0>(i) << ' ' << std::get<1>(i) << ' ' << std::get<2>(i) << endl;
//        }

    static std::map<std::string, std::function<FormatItem::ptr(const std::string& str)> > s_format_items = {
#define XX(str, C) \
        {#str, [](const std::string& fmt) { return FormatItem::ptr(new C(fmt));}}

            XX(m, MessageFormatItem),           //m:消息
            XX(p, LevelFormatItem),             //p:日志级别
            XX(r, ElapseFormatItem),            //r:累计毫秒数
            XX(c, NameFormatItem),              //c:日志名称
            XX(t, ThreadIdFormatItem),          //t:线程id
            XX(n, NewLineFormatItem),           //n:换行
            XX(d, TimeFormatItem),          //d:时间
            XX(f, FileFormatItem),          //f:文件名
            XX(l, LineFormatItem),              //l:行号
            XX(T, TabFormatItem),               //T:Tab
            XX(F, FiberIdFormatItem),           //F:协程id
            XX(N, ThreadNameFormatItem),        //N:线程名称
#undef XX
    };


    for(auto& i : vec) {
        if(std::get<2>(i) == 0) {
            m_items.push_back(FormatItem::ptr(new StringFormatItem(std::get<0>(i))));
        } else {
            auto it = s_format_items.find(std::get<0>(i));
            if(it == s_format_items.end()) {
                m_items.push_back(FormatItem::ptr(new StringFormatItem("<<error_format %" + std::get<0>(i) + ">>")));
            } else {
                m_items.push_back(it->second(std::get<1>(i)));
            }
        }
    }


}
void Formatter::format(std::ostream &os, const Event::ptr &event)     {
    for(auto& i:m_items)
    {
        i->format(os, event);
    }
}