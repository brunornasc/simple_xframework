//
// Created by bruno on 11/02/2026.
//

#include <X11/Xutil.h>
#include "include/Form.h"
#include "include/System.h"
#include "include/Component.h"
#include "include/utils/Utils.h"

namespace System {

    void Form::init() {

    }

    Form::Form() {
        this->display = XOpenDisplay(nullptr);
        this->screen = DefaultScreen(this->display);
        this->windowBgColor = WhitePixel(this->display, this->screen);
        this->windowFgColor = BlackPixel(this->display, this->screen);
        this->colorsAllocated = false;
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
        Theme* theme = ThemeManager::getCurrentTheme();
        Display* dpy = this->display;
        int scr = this->screen;
        Colormap cmap = DefaultColormap(dpy, scr);

        // Window Background
        unsigned long themeBg = theme->getWindowBackgroundColor();
        XColor xcolor;
        Utils::parseXColor(&xcolor, themeBg);

        if (!XAllocColor(dpy, cmap, &xcolor)) {
            std::cerr << "FALHA ao alocar cor de fundo! Usando fallback" << std::endl;
            this->windowBgColor = WhitePixel(dpy, scr);

        } else {
            this->windowBgColor = xcolor.pixel;

        }

        this->window = XCreateSimpleWindow(
            this->display,
            RootWindow(this->display, this->screen),
            this->location->left, this->location->top,
            this->size->width, this->size->height, 1,
            BlackPixel(this->display, this->screen),
            this->windowBgColor
        );

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
        static Component* lastHovered = nullptr;

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

                case MotionNotify: {
                    int x = e.xmotion.x;
                    int y = e.xmotion.y;

                    Component* currentHovered = getComponentAt(x, y);

                    if (currentHovered != lastHovered) {
                        if (lastHovered) {
                            lastHovered->_onMouseLeave();
                        }
                        if (currentHovered) {
                            currentHovered->_onMouseEnter();
                        }
                        lastHovered = currentHovered;
                    }

                    if (currentHovered) {
                        currentHovered->_onMouseMove(&e.xbutton);
                    }
                    else if (this->OnMouseOver != nullptr) {
                        this->OnMouseOver->execute(&e);
                    }

                    break;
                }

                case ButtonPress: {
                    Component* clicked = getComponentAt(e.xbutton.x, e.xbutton.y);

                    if (clicked) {
                        clicked->_onMousePress(&e.xbutton);

                    } else if (this->OnMouseClick != nullptr) {
                        this->OnMouseClick->execute(&e.xbutton);

                    }

                    break;
                }

                default:
                    break;
            }
            
            XNextEvent(this->display, &e);
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

    Component* Form::getComponentAt(int x, int y) {
        for (auto it = components.rbegin(); it != components.rend(); ++it) {
            Component* comp = *it;

            // Verifica se o ponto está dentro do componente
            if (x >= comp->getLeft() &&
                x <= comp->getLeft() + comp->getWidth() &&
                y >= comp->getTop() &&
                y <= comp->getTop() + comp->getHeight()) {
                return comp;
                }
        }
        return nullptr;
    }

}
