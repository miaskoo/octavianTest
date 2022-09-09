#include "bufferComponent.h"
#include "glFunctional.h"

void bufferComponent::bind() const {
    glForwarder::bindVertexArray(buffIdx);
}

void bufferComponent::unbind() const {
    glForwarder::unbindVertexArray();
}

void bufferComponent::use() const {
    setCullface(true);
    glForwarder::drawTriangleElements(buffSize);
    setCullface(false);
}

void bufferComponent::setCullface(bool value) const {
    glForwarder::setCullfaceMode(value);
}

void bufferComponent::setBuffIdx(unsigned int aBuffIdx) {
    buffIdx = aBuffIdx;
    markDirty();
}

void bufferComponent::setBuffSize(unsigned int aBuffSize) {
    buffSize = aBuffSize;
    markDirty();
}
