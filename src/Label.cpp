#include "include/Label.h"
#include "include/Form.h"
#include <cstring>

#include "include/theme/Theme.h"
#include "include/theme/ThemeManager.h"
#include "include/utils/Utils.h"

namespace System {

    Label::~Label() {
        if (this->fontInfo) {
            XFreeFont(this->parent->getDisplay(), this->fontInfo);
        }
        XFreeGC(this->parent->getDisplay(), this->gc);
        delete this->location;
        delete this->size;
    }

    void Label::setText(const std::string &text) {
        this->text = text;
    }

    void Label::setLocation(int x, int y) {
        this->location->left = x;
        this->location->top = y;
    }

    void Label::setSize(int width, int height) {
        this->size->width = width;
        this->size->height = height;
    }

    void Label::create() {
        this->gc = XCreateGC(this->parent->getDisplay(), this->parent->getWindow(), 0, nullptr);

        this->fontInfo = XLoadQueryFont(this->parent->getDisplay(), "fixed");
        if (!this->fontInfo) {
            this->fontInfo = XLoadQueryFont(this->parent->getDisplay(), "9x15");
        }
        if (this->fontInfo) {
            XSetFont(this->parent->getDisplay(), this->gc, this->fontInfo->fid);
        }

        Theme *theme = ThemeManager::getCurrentTheme();
        XColor xcolor;
        Utils::parseXColor(&xcolor, theme->getLabelTextColor());

        this->foregroundColor = BlackPixel(this->parent->getDisplay(), this->parent->getScreen()); //fallback
        auto cmap= DefaultColormap(this->parent->getDisplay(), this->parent->getScreen());

        if (XAllocColor(this->parent->getDisplay(), cmap, &xcolor)) {
            this->foregroundColor = xcolor.pixel;
        }

        XSetForeground(this->parent->getDisplay(), this->gc, this->foregroundColor);
    }

    void Label::draw() {
        if (this->text.empty()) return;

        int text_width = XTextWidth(this->fontInfo, this->text.c_str(), this->text.length());
        int font_height = this->fontInfo->ascent + this->fontInfo->descent;

        int x = this->location->left;
        int y = this->location->top + this->fontInfo->ascent;

        XDrawString(this->parent->getDisplay(), this->parent->getWindow(), this->gc,
                   x, y, this->text.c_str(), this->text.length());
    }
}
