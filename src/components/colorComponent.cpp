#include "colorComponent.h"
#include "glFunctional.h"

void colorComponent::bind() const {
    if (alphaMode) {
        glForwarder::setAlphaMode(true);
    }
}

void colorComponent::unbind() const {
    if (alphaMode) {
        glForwarder::setAlphaMode(false);
    }
}

void colorComponent::use() const {
    if (alphaMode) {
        glForwarder::setColor4(color[0], color[1], color[2], color[3]);
    }
    else {
        glForwarder::setColor3(color[0], color[1], color[2]);
    }
}

void colorComponent::setColor(unsigned int r, unsigned int g, unsigned int b) {
    color[0] = r;
    color[1] = g;
    color[2] = b;
    markDirty();
}

void colorComponent::setColor(unsigned int r, unsigned int g, unsigned int b, unsigned int a) {
    color[0] = r;
    color[1] = g;
    color[2] = b;
    color[3] = a;
    markDirty();
}

void colorComponent::setAlphaMode(bool value) {
    alphaMode = value;
    markDirty();
}
