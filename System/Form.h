
#include <iostream>

#include "Types.h"
#include "Object.h"
#include "EventHandlers.h"

#ifndef __DEBUG_H__
#include "Debug.h"
#endif

#ifndef __FORM_H__

namespace System {

    class Form : public Object {
        public: System::Location *location;
        public: System::Size *size;
        private: bool visible;
        public: char* title;
        private: Display *display;
        private: int screen;
        private: Window window;
        private: void WindowEventHandler();
        private: void Destroy();        
        
        public:
        Form();  
        ~Form();

        virtual void InitializeComponent() { }
        virtual void Dispose() { }    

        void Move(Location *);
        void setSize(Size *);
        void setSize(unsigned int, unsigned int);
        void setTitle(char *);
        void setVisible(bool);
        void Close();
        void Create();

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

namespace System {

    Form::Form() {
        
        this->OnExpose = NULL;
        this->OnDestroy = NULL;
        this->OnConfigureNotify = NULL;
        this->OnCreate = NULL;
        this->OnFocus = NULL;
        this->OnLostFocus = NULL;
        this->OnConfigureNotify = NULL;
        this->OnMove = NULL;
        this->OnDraw = NULL;
        this->OnKeyPress = NULL;
        this->OnKeyRelease = NULL;
        this->OnMouseClick = NULL;
        this->OnMouseRelease = NULL;
        this->OnMouseOver = NULL;

        this->size = new System::Size();
        this->location = new System::Location();
        this->location->Left = 0;
        this->location->Top = 0;

        this->size->Height = 150;
        this->size->Width = 150;
        
        this->visible = true;

        this->display = XOpenDisplay(NULL);
        this->screen = DefaultScreen(this->display);

    }

    void Form::Create() {
        this->window = XCreateSimpleWindow(this->display, RootWindow(this->display, this->screen),
            this->location->Left, this->location->Top, this->size->Width, this->size->Height, 1,
                BlackPixel(this->display, this->screen), WhitePixel(this->display, this->screen));
                
        //Prevent error on close window
        Atom delWindow = XInternAtom(this->display, "WM_DELETE_WINDOW", 0);
        XSetWMProtocols(this->display , this->window, &delWindow, 1);
        
        XStoreName(this->display, this->window, this->title);
        XMapWindow(this->display, this->window);

        XSelectInput(this->display, this->window,
                ExposureMask |
                StructureNotifyMask |
                FocusChangeMask |
                PointerMotionMask |
                ButtonPressMask |
                ButtonReleaseMask |
                KeyPressMask |
                KeyReleaseMask);
                
        this->WindowEventHandler();
    }

    Form::~Form() {
        this->Dispose();
    }

    void Form::WindowEventHandler() {
        EventArgs e;

        XNextEvent(this->display, &e);

        while(e.type != ClientMessage) {

            switch(e.type) {
                case Expose:
                    if(this->OnExpose != NULL) this->OnExpose->Execute(&e);
                    //XFillRectangle(this->display, this->window, DefaultGC(this->display, this->screen), 20, 20, 50, 50);
                break;

                case ButtonPress:
                    if(this->OnMouseClick != NULL)
                        this->OnMouseClick->Execute(&e.xbutton);
                break;

                // case UnmapNotify: //Minimize
                //
                // break;
                //
                // case MapNotify: // Restore
                //
                // break;
                //
                // case ConfigureNotify: //Move/Resize
                //
                // break;

                case MotionNotify:
                    if(this->OnMouseOver != NULL)
                        this->OnMouseOver->Execute(&e);

                break;

                default:
                    break;
            }
            
            XNextEvent(this->display, &e);
            //cout << e.type << endl;
        }
        
        this->Destroy();
    }

    void Form::Destroy() {
        EventArgs e;
        if(this->OnDestroy != NULL) this->OnDestroy->Execute(&e);

        XDestroyWindow(this->display, this->window);
        XCloseDisplay(this->display);
    }

    void Form::Move(Location *l) {
        this->location->Left = l->Left;
        this->location->Top = l->Top;
    }

    void Form::setSize(Size *s) {
        this->size->Height = s->Height;
        this->size->Width = s->Width;
    }

    void Form::setTitle(char *t) {
        //this->title = t;
        //XStoreName(this->display, this->window, t);

        //call change title
    }

    void Form::setVisible(bool _v) {
         this->visible = _v;

         //call setvisible x

    }

    void Form::Close() {
        this->Destroy();
    }

}




#define __FORM_H__
#endif