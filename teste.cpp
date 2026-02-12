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
        this->title = (char *) "Janela 1 - KDE Theme";

        // Label com tema
        this->label1 = new System::Label(this);
        this->label1->setText("Label com tema KDE!");
        this->label1->setLocation(50, 50);
        this->addComponent(this->label1);

        // Button com tema
        this->button1 = new System::Button(this);
        this->button1->setText("Clique Aqui");
        this->button1->setLocation(50, 100);
        this->addComponent(this->button1);

        // Event handlers
        this->OnMouseClick = new System::MouseEventHandler<Janela1>(this, &Janela1::click_window);
    }

    void click_window(System::MouseEventArgs *e) {
        std::cout << "Mouse clicked!" << std::endl;
    }
};

int main() {
    auto *app = new Janela1();
    app->create();
    app->run();
    return 0;
}