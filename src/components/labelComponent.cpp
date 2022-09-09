#include "labelComponent.h"
#include "glFunctional.h"

void labelComponent::use() const {
    glForwarder::drawBitmapText(text.c_str(), text.length(), font);
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

