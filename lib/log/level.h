//
// Created by wyatt on 2022/3/27.
//

#ifndef LOG_LEVEL_H
#define LOG_LEVEL_H

namespace wyatt{
    class Level {
    public:
        enum level{
            DEBUG,INFO,WARN,ERROR,FATAL
        };

        static const char* toString(Level::level level);
    };

}

#endif //LOG_LEVEL_H
