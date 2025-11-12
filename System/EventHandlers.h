//retirado de https://blog.molecular-matters.com/2011/09/19/generic-type-safe-delegates-and-events-in-c/

#include "Types.h"

#ifndef __EVENT_HANDLERS_H__
#define __EVENT_HANDLERS_H__

namespace System {

  class EventHandler {
    public:
    virtual void Execute(System::MouseEventArgs* e) {};
    virtual void Execute(System::KeyEventArgs* e) {};
    virtual void Execute(System::EventArgs* e) {};
    };
    
#pragma region EventHandlers

    template <class C>
    class MouseEventHandler : public EventHandler {
        typedef void (C::*MemberFunction)(System::MouseEventArgs*);
        
        public:
        MouseEventHandler(MemberFunction memberFunction)   {
            m_memberFunction = memberFunction;
        }
        
        virtual void Execute(System::MouseEventArgs* e) {
            (m_instance->*m_memberFunction)(e);
        }
        
        private:
        C* m_instance;
        MemberFunction m_memberFunction;
    };

    template <class C>
    class KeyEventHandler : public EventHandler {
        typedef void (C::*MemberFunction)(System::KeyEventArgs*);
        
        public:
        KeyEventHandler(MemberFunction memberFunction)   {
            m_memberFunction = memberFunction;
        }
      
        virtual void Execute(System::KeyEventArgs* e) {
            (m_instance->*m_memberFunction)(e);
        }
        
        private:
        C* m_instance;
        MemberFunction m_memberFunction;
    };

    template <class C>
    class WindowEventHandler : public EventHandler {
        typedef void (C::*MemberFunction)(System::EventArgs*);
        
        public:
        WindowEventHandler(MemberFunction memberFunction)   {
            m_memberFunction = memberFunction;
        }
      
        virtual void Execute(System::EventArgs* e) {
            (m_instance->*m_memberFunction)(e);
        }
        
        private:
        C* m_instance;
        MemberFunction m_memberFunction;
    };



#pragma endregion
}

#endif

/*
    class AbstractBase {
    public:
    virtual void CallFunction(int ARG0) = 0;
    };
    
    template <class C>
    class MemberFunctionWrapper : public AbstractBase {
    typedef void (C::*MemberFunction)(int);
    
    public:
    MemberFunctionWrapper(MemberFunction memberFunction)   {
        m_memberFunction = memberFunction;
    }
    
    virtual void CallFunction(int ARG0) {
        (m_instance->*m_memberFunction)(ARG0);
    }
    
    private:
    C* m_instance;
    MemberFunction m_memberFunction;
    };

*/
