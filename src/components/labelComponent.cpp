#include "labelComponent.h"
#include "freeglut/glut.h"

void labelComponent::use() const {
    int xChar = 0;
    for (auto& c : text) {
        glRasterPos2f(xChar, 0);
        glutBitmapCharacter(font, c);
        xChar = xChar + glutBitmapWidth(font, c);
    }
}

void labelComponent::setText(const std::string &aText) {
    text = aText;
    markDirty();
}

void labelComponent::setFont(void *aFont) {
    if (aFont == NULL) {
        return;
    }
    font = aFont;
    markDirty();
}

const std::string& labelComponent::getText() const {
    return text;
}

void* labelComponent::getFont() const {
    return font;
}

