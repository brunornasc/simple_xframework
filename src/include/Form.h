
#pragma once

#include <vector>

#include "Types.h"
#include "Object.h"
#include "EventHandlers.h"

#ifndef DEBUG_H_
    #include "Debug.h"
#endif

#ifndef FORM_H_
#define FORM_H_

namespace System {
    class Component;

    class Form : public Object {
    friend class Component;

    protected:
        bool visible;
        Display *display;
        int screen;
        Window window{};
        void windowEventHandler();
        void destroy() const;
        static void init();

    void createComponents() const;

public:
        std::vector<Component*> components;
        System::Location *location;
        System::Size *size;
        char* title{};
        char* iconPath{};

        Form();  
        ~Form() override;

        virtual void initializeComponent() { }
        virtual void dispose() { }

        void move(Location *) const;
        void setSize(Size *) const;
        void setSize(unsigned int, unsigned int);
        void setTitle(char *);
        void setVisible(bool);
        void setBackgroundColor(unsigned long);
        void setBackgroundColor(char *);
        void setIcon(char *);
        void close() const;
        void create();
        void addComponent(Component *component);
        Display* getDisplay() const { return display; }
        Window getWindow() const { return window; }
        int getScreen() const { return screen; }
        void run();

        System::EventHandler *OnExpose;
        System::EventHandler *OnDestroy;
        System::EventHandler *OnCreate;
        System::EventHandler *OnFocus;
        System::EventHandler *OnLostFocus;
        System::EventHandler *OnConfigureNotify;
        System::EventHandler *OnMove;
        System::EventHandler *OnDraw;
        System::EventHandler *OnKeyPress;
        System::EventHandler *OnKeyRelease;
        //use this->OnMouseClick = new System::MouseEventHandler<CLASS>(&CLASS::METHOD);
        System::EventHandler *OnMouseClick;
        System::EventHandler *OnMouseRelease;
        System::EventHandler *OnMouseOver;
    };
}
#endif