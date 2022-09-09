#include "textureComponent.h"
#include "glFunctional.h"

void textureComponent::setTexIdx(unsigned int aTexIdx) {
    texIdx = aTexIdx;
    markDirty();
}

void textureComponent::setShaderIdx(unsigned int aShaderIdx) {
    shaderIdx = aShaderIdx;
    markDirty();
}

void textureComponent::bind() const {
    glForwarder::bind2DTexture(texIdx);
    glForwarder::bindShaderProgram(shaderIdx);
}

void textureComponent::unbind() const {
    glForwarder::unbindShaderProgram();
    glForwarder::unbind2DTexture();
}
