//
// Created by wyatt on 2022/3/30.
//

#include "thread.h"

namespace wyatt
{
    thread_local Thread* Thread::this_thread;
    thread_local std::string Thread::this_name;

}
