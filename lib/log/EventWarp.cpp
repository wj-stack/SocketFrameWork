//
// Created by wyatt on 2022/3/28.
//

#include "EventWarp.h"

std::ostream &EventWarp::getSs() {
    return ss;
}

EventWarp::~EventWarp()    {
    auto event =make_shared<Event>(ss.str(), level, name, until::GetThreadId(),
                                   file, line, 0 , "");
    logger->log(event);
}