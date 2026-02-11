#pragma once

#include <iostream>

#ifndef DEBUG_H_
#define DEBUG_H_

namespace System {
    class Debug {
        public:
        static void print(char *c) {
            std::cout << c << std::endl;
        }
    };

}

#endif