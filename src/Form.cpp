//
// Created by bruno on 11/02/2026.
//

#include "include/Form.h"
#include "include/System.h"
#include "include/Component.h"
#include <X11/Xutil.h>

namespace System {
    std::string CurrentDesktopEnvironment;

    void Form::init() {
        DetectDesktopEnvironment();
    }

    Form::Form() {
        this->OnExpose = nullptr;
        this->OnDestroy = nullptr;
        this->OnConfigureNotify = nullptr;
        this->OnCreate = nullptr;
        this->OnFocus = nullptr;
        this->OnLostFocus = nullptr;
        this->OnConfigureNotify = nullptr;
        this->OnMove = nullptr;
        this->OnDraw = nullptr;
        this->OnKeyPress = nullptr;
        this->OnKeyRelease = nullptr;
        this->OnMouseClick = nullptr;
        this->OnMouseRelease = nullptr;
        this->OnMouseOver = nullptr;

        this->size = new System::Size();
        this->location = new System::Location();
        this->location->left = 0;
        this->location->top = 0;

        this->size->height = 150;
        this->size->width = 150;
        
        this->visible = true;

        this->display = XOpenDisplay(nullptr);
        this->screen = DefaultScreen(this->display);
    }

    Form::~Form() {
        for (auto component : components) {
            delete component;
        }

        components.clear();

        if (this->display) {
            XCloseDisplay(this->display);
            this->display = nullptr;
        }

        delete this->size;
        delete this->location;
    }

    void Form::create() {
        Form::init();
        this->initializeComponent();

        this->window = XCreateSimpleWindow(this->display, RootWindow(this->display, this->screen),
            this->location->left, this->location->top, this->size->width, this->size->height, 1,
                BlackPixel(this->display, this->screen), WhitePixel(this->display, this->screen));

        this->createComponents();

        //Prevent error on close window
        Atom delWindow = XInternAtom(this->display, "WM_DELETE_WINDOW", 0);
        XSetWMProtocols(this->display , this->window, &delWindow, 1);
        
        XStoreName(this->display, this->window, this->title);

        if (this->iconPath != nullptr) {
            this->setIcon(this->iconPath);
        }

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
    }

    void Form::run() {
        this->windowEventHandler();
    }

    void Form::windowEventHandler() {
        EventArgs e;

        XNextEvent(this->display, &e);

        while(e.type != ClientMessage) {

            switch(e.type) {
                case Expose:
                    if(this->OnExpose != nullptr)
                        this->OnExpose->execute(&e);

                    if (!components.empty()) {
                        for (const auto component : components) {
                            component->draw();
                        }
                    }
                break;

                case ButtonPress:
                    if(this->OnMouseClick != nullptr)
                        this->OnMouseClick->execute(&e.xbutton);
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
                    if(this->OnMouseOver != nullptr)
                        this->OnMouseOver->execute(&e);

                break;

                default:
                    break;
            }
            
            XNextEvent(this->display, &e);
            //cout << e.type << endl;
        }
        
        this->destroy();
    }

    void Form::destroy() const {
        EventArgs e;
        if(this->OnDestroy != nullptr) this->OnDestroy->execute(&e);

        XDestroyWindow(this->display, this->window);
        XCloseDisplay(this->display);
    }

    void Form::move(Location *l) const {
        this->location->left = l->left;
        this->location->top = l->top;
        XMoveWindow(this->display, this->window, l->left, l->top);
    }

    void Form::setSize(Size *s) const {
        this->size->height = s->height;
        this->size->width = s->width;
    }

    void Form::setTitle(char *t) {
        this->title = t;
        XStoreName(this->display, this->window, t);
    }

    void Form::setVisible(const bool _v) {
         this->visible = _v;
         if (_v) {
             XMapWindow(this->display, this->window);
         } else {
             XUnmapWindow(this->display, this->window);
         }
    }

    void Form::setBackgroundColor(unsigned long color) {
        XSetWindowBackground(this->display, this->window, color);
        XClearArea(this->display, this->window, 0, 0, 0, 0, True);
    }

    void Form::setBackgroundColor(char *color) {
        if (!this->display) {
            std::cerr << "Erro: Display não inicializado!" << std::endl;
            return;
        }

        XColor xcolor;
        Colormap colormap = DefaultColormap(this->display, this->screen);

        if (!XParseColor(this->display, colormap, color, &xcolor)) {
            std::cerr << "Erro: Não foi possível parsear a cor: " << color << std::endl;
            return;
        }

        if (!XAllocColor(this->display, colormap, &xcolor)) {
            std::cerr << "Erro: Não foi possível alocar a cor: " << color << std::endl;
            return;
        }

        this->setBackgroundColor(xcolor.pixel);
    }

    void Form::setIcon(char *path) {
        this->iconPath = path;
        Pixmap icon;
        unsigned int width, height;
        int hot_x, hot_y;

        if (XReadBitmapFile(this->display, this->window, path, &width, &height, &icon, &hot_x, &hot_y) == BitmapSuccess) {
             XWMHints *hints = XAllocWMHints();
             hints->flags = IconPixmapHint | IconMaskHint;
             hints->icon_pixmap = icon;
             XSetWMHints(this->display, this->window, hints);
             XFree(hints);
        }
    }

    void Form::close() const {
        this->destroy();
    }

    void Form::addComponent(Component *component) {
        this->components.push_back(component);
    }

    void Form::createComponents() const {
        for (const auto component : components) {
            component->create();
        }
    }

}
