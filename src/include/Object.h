#ifndef OBJECT_H_
#define OBJECT_H_
namespace System {

    class Object {
    public:
        virtual ~Object() = default;
        virtual bool equals(Object *o) {
            if (o == this) return true;
            return false;
        }
        
    };
}

#endif