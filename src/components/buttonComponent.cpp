#include "buttonComponent.h"
#include "stdio.h"

void buttonComponent::bind() const {
    auto id = static_cast<size_t>(state);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texIdx[id]);
}

void buttonComponent::setTexIdx(GLuint aTexIdxWait, GLuint aTexIdxHover, GLuint aTexIdxPress) {
    texIdx[0] = aTexIdxWait;
    texIdx[1] = aTexIdxHover;
    texIdx[2] = aTexIdxPress;
    markDirty();
}

stateButton buttonComponent::getState() {
    return state;
}

void buttonComponent::setState(stateButton aState) {
    state = aState;
    markDirty();
}
