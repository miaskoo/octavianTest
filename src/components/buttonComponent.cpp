#include "buttonComponent.h"
#include "glFunctional.h"

void buttonComponent::bind() const {
    auto id = static_cast<unsigned int>(state);
    glForwarder::bind2DTexture(texIdx[id]);
}

void buttonComponent::setTexIdx(unsigned int aTexIdxWait, unsigned int aTexIdxHover, unsigned int aTexIdxPress) {
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
