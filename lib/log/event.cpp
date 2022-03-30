//
// Created by wyatt on 2022/3/27.
//

#include "event.h"

namespace wyatt{
    std::chrono::time_point<std::chrono::system_clock, std::chrono::microseconds>  Event::startTime =
            std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::system_clock::now());

}