//
// Created by wyatt on 2022/3/27.
//

#include "level.h"

const char* Level::toString(Level::level level) {
    const char *str[] = {"DEBUG", "INFO", "WARN", "ERROR", "FATAL"};
    return str[level];
}
