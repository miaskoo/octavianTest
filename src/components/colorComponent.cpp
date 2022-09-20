#include "colorComponent.h"
#include "glFunctional.h"

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

bool colorComponent::isAlphaMode() {
    return alphaMode;
}

unsigned int* colorComponent::getColor() {
    return color;
}
