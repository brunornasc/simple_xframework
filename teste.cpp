#include "src/include/System.h"
#include "src/include/Label.h"
#include <iostream>

//g++ -o teste teste.cpp src/Form.cpp src/Label.cpp -lX11

class Janela1 : public System::Form {
public:
    System::Label *label1;

    void initializeComponent() override {
        this->size->height = 500;
        this->size->width = 500;
        this->location->left = 200;
        this->location->top = 500;
        this->title = (char *) "Janela 1";
        this->OnMouseClick =  new System::MouseEventHandler<Janela1>(this, &Janela1::click_window);
        this->OnDestroy = new System::WindowEventHandler<Janela1>(this, &Janela1::window_destroyed);
        this->OnMouseOver = new System::WindowEventHandler<Janela1>(this, &Janela1::window_mouseover);

        this->label1 = new System::Label(this);
        this->label1->setText("Label 1");
        this->label1->setLocation(50, 50);
        this->addComponent(this->label1);
    }

    void window_mouseover(System::EventArgs * e) {
        std::cout << "X: " << e->xmotion.x << " - Y: " << e->xmotion.y << std::endl;
    }

    void click_window(System::MouseEventArgs *e) {
        std::cout << "mouse" << std::endl;
        this->setBackgroundColor("#FFFFFF");
    }

    void window_destroyed(System::EventArgs *e) {
        std::cout << "destroyed" << std::endl;
    }

    void Dispose() {

    }
};

int main (int argc, char **argv) {

    auto *app = new Janela1();
    app->create();
    app->setBackgroundColor("#00FF00");
    app->run();

    return 0;
}
