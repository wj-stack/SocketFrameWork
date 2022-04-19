//
// Created by wyatt on 2022/4/1.
//

#ifndef SOCKETFRAMEWORK_FIBER_H
#define SOCKETFRAMEWORK_FIBER_H

#include <memory>
#include <ucontext.h>
#include "../thread/thread.h"
#include "../thread/mutex.h"

class Fiber : public std::enable_shared_from_this<Fiber>{

};


#endif //SOCKETFRAMEWORK_FIBER_H
