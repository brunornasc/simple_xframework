#ifndef __OBJECT_H__

namespace System {

    class Object {
        virtual bool Equals(Object *o) {
            if (o == this) return true;
            return false;
        }
        
    };
}
#define __OBJECT_H__
#endif