//
// Created by bruno on 11/02/2026.
//

#ifndef SIMPLE_XFRAMEWORK_COMPONENT_H
#define SIMPLE_XFRAMEWORK_COMPONENT_H
#include "Object.h"

namespace System {
    class Component : public Object {
    public:
        virtual void draw() = 0;
        virtual ~Component() = default;
    };
}
#endif //SIMPLE_XFRAMEWORK_COMPONENT_H