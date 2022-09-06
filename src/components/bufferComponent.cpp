#include "bufferComponent.h"
#include "GLUT/glut.h"

void bufferComponent::bind() const {
    glBindVertexArrayAPPLE(buffIdx);
}

void bufferComponent::unbind() const {
    glBindVertexArrayAPPLE(0);
}

void bufferComponent::use() const {
    setCullface(true);
    glDrawElements(GL_TRIANGLES, buffSize, GL_UNSIGNED_BYTE, 0);
    setCullface(false);
}

void bufferComponent::setCullface(bool value) const {
    if (value) {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
    }
    else {
        glDisable(GL_CULL_FACE);
    }
}

void bufferComponent::setBuffIdx(GLuint aBuffIdx) {
    buffIdx = aBuffIdx;
    markDirty();
}

void bufferComponent::setBuffSize(GLuint aBuffSize) {
    buffSize = aBuffSize;
    markDirty();
}
