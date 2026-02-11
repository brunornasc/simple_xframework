//retirado de https://blog.molecular-matters.com/2011/09/19/generic-type-safe-delegates-and-events-in-c/

#pragma once

#include "Types.h"

#ifndef EVENT_HANDLERS_H_
#define EVENT_HANDLERS_H_

namespace System {

  class EventHandler {
    public:
        virtual void execute(System::MouseEventArgs* e) {}
        virtual void execute(System::KeyEventArgs* e) {}
        virtual void execute(System::EventArgs* e) {}
        virtual ~EventHandler() = default;
    };
    
#pragma region EventHandlers

    template <class C>
    class MouseEventHandler : public EventHandler {
    private:
        typedef void (C::*MemberFunction)(System::MouseEventArgs*);
        C* m_instance;
        MemberFunction m_memberFunction;
        
    public:
        MouseEventHandler(C *instance, const MemberFunction memberFunction) : m_instance(instance),
                                                            m_memberFunction(memberFunction) { }
        
        void execute(System::MouseEventArgs* e) override {
            (m_instance->*m_memberFunction)(e);
        }

    };

    template <class C>
    class KeyEventHandler : public EventHandler {
    private:
        typedef void (C::*MemberFunction)(System::KeyEventArgs*);
        C* m_instance;
        MemberFunction m_memberFunction;

        
    public:
        KeyEventHandler(C *instance, const MemberFunction memberFunction): m_instance(instance),
                                                            m_memberFunction(memberFunction) { }
      
        void execute(System::KeyEventArgs* e) override {
            (m_instance->*m_memberFunction)(e);
        }

    };

    template <class C>
    class WindowEventHandler : public EventHandler {
    private:
        typedef void (C::*MemberFunction)(System::EventArgs*);
        C* m_instance;
        MemberFunction m_memberFunction;

        
        public:
        WindowEventHandler(C *instance, const MemberFunction memberFunction) : m_instance(instance),
                                                            m_memberFunction(memberFunction) { }
      
        void execute(System::EventArgs* e) override {
            (m_instance->*m_memberFunction)(e);
        }

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
