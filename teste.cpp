#include "System/System.h"
#include <iostream>

//g++ -o teste teste.cpp -lX11


class Janela1 : public System::Form {
    public:

    void InitializeComponent() override {
        this->size->Height = 500;
        this->size->Width = 500;
        this->location->Left = 200;
        this->location->Top = 500;
        this->title = (char *) "Janela 1";
        this->OnMouseClick =  new System::MouseEventHandler<Janela1>(&Janela1::click_window);
        this->OnDestroy = new System::WindowEventHandler<Janela1>(&Janela1::window_destroyed);
        this->OnMouseOver = new System::WindowEventHandler<Janela1>(&Janela1::window_mouseover);
    }

    void window_mouseover(System::EventArgs * e) {
        std::cout << "X: " << e->xmotion.x << " - Y: " << e->xmotion.y << std::endl;
    }

    void click_window(System::MouseEventArgs *e) {
        std::cout << "mouse" << std::endl;
    }

    void window_destroyed(System::EventArgs *e) {
        std::cout << "destroyed" << std::endl;
    }

    void Dispose() {

    }
};

int main (int argc, char **argv) {

    Janela1 *app = new Janela1();
    app->InitializeComponent();
    app->Create();

    return 0;
}
