#include "colorComponent.h"
#include "GLUT/glut.h"

void colorComponent::bind() const {
    if (alphaMode) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
}

void colorComponent::unbind() const {
    if (alphaMode) {
        glDisable(GL_BLEND);
    }
}

void colorComponent::use() const {
    if (alphaMode) {
        glColor4f(color[0], color[1], color[2], color[3]);
    }
    else {
        glColor3f(color[0], color[1], color[2]);
    }
}

void colorComponent::setColor(float r, float g, float b) {
    color[0] = r;
    color[1] = g;
    color[2] = b;
    markDirty();
}

void colorComponent::setColor(float r, float g, float b, float a) {
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
