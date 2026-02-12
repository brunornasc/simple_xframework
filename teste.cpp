#include "src/include/System.h"
#include <iostream>

class Janela1 : public System::Form {
public:
    System::Label *label1;
    System::Button *button1;

    void initializeComponent() override {
        this->size->height = 500;
        this->size->width = 500;
        this->location->left = 200;
        this->location->top = 500;
        this->title = (char *) "Janela 1";

        // Label com tema
        this->label1 = new System::Label(this);
        this->label1->setText("Label");
        this->label1->setLocation(50, 50);
        this->addComponent(this->label1);

        // Button com tema
        this->button1 = new System::Button(this);
        this->button1->setText("Clique Aqui");
        this->button1->setLocation(50, 100);
        this->button1->onClick = new System::MouseEventHandler<Janela1>(this, &Janela1::button1_click);
        this->button1->onMouseMove = new System::MouseEventHandler<Janela1>(this, &Janela1::button1_hover);
        this->addComponent(this->button1);

        // Event handlers
        this->OnMouseClick = new System::MouseEventHandler<Janela1>(this, &Janela1::click_window);
    }

    void click_window(System::MouseEventArgs *e) {
        std::cout << "window clicked!" << std::endl;
    }

    void button1_click(System::MouseEventArgs *e) {
        std::cout << "button clicked!" << std::endl;
    }

    void button1_hover(System::MouseEventArgs *e) {
        std::cout << "button hovered -> x:" << e->x << " y:" << e->y << std::endl;
    }
};

int main() {
    auto *app = new Janela1();
    app->create();
    app->run();
    return 0;
}