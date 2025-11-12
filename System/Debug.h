#include <iostream>

#ifndef __DEBUG_H__
#define __DEBUG_H__

namespace System {
    class Debug {
        public:
        static void Print(char *c) {
            std::cout << c << std::endl;
        }
    };

}

#endif