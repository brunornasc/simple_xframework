#include "include/Label.h"
#include <cstring>

namespace System {

    Label::Label(Display *display, Window parent, int screen) {
        this->display = display;
        this->parent = parent;
        this->screen = screen;
        this->location = new System::Location();
        this->size = new System::Size();
        this->location->left = 0;
        this->location->top = 0;
        this->size->width = 100;
        this->size->height = 20;

        this->gc = XCreateGC(display, parent, 0, nullptr);

        // Load default font
        this->fontInfo = XLoadQueryFont(display, "fixed");
        if (!this->fontInfo) {
            this->fontInfo = XLoadQueryFont(display, "9x15");
        }
        if (this->fontInfo) {
            XSetFont(display, this->gc, this->fontInfo->fid);
        }

        // Default color (black)
        this->foregroundColor = BlackPixel(display, screen);
        XSetForeground(display, this->gc, this->foregroundColor);
    }

    Label::~Label() {
        if (this->fontInfo) {
            XFreeFont(this->display, this->fontInfo);
        }
        XFreeGC(this->display, this->gc);
        delete this->location;
        delete this->size;
    }

    void Label::setText(const std::string &text) {
        this->text = text;
        this->draw();
    }

    void Label::setLocation(int x, int y) {
        this->location->left = x;
        this->location->top = y;
        this->draw();
    }

    void Label::setSize(int width, int height) {
        this->size->width = width;
        this->size->height = height;
        this->draw();
    }

    void Label::draw() {
        // Clear the area before drawing (simple approach, might flicker)
        // Ideally, we should handle Expose events properly in the parent container
        // For now, we just draw the text

        // Note: In a real retained mode framework, we wouldn't draw directly here.
        // Instead, we would invalidate the region and wait for a paint event.
        // However, given the current structure, we'll draw directly for simplicity.

        if (this->text.empty()) return;

        int text_width = XTextWidth(this->fontInfo, this->text.c_str(), this->text.length());
        int font_height = this->fontInfo->ascent + this->fontInfo->descent;

        // Simple centering or just drawing at location
        int x = this->location->left;
        int y = this->location->top + this->fontInfo->ascent; // Baseline

        XDrawString(this->display, this->parent, this->gc, x, y, this->text.c_str(), this->text.length());
    }
}
