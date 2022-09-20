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

unsigned int textureComponent::getTexIdx() const {
    return texIdx;
}

unsigned int textureComponent::getShaderIdx() const {
    return shaderIdx;
}
