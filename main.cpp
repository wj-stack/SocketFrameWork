#include <iostream>
#include "lib/config/config.h"
#include "lib/log/loggerManager.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    WYATT_LOG_ROOT_DEBUG() << "test" << endl;
    return 0;
}
