#include "textureComponent.h"

void textureComponent::setTexIdx(GLuint aTexIdx) {
    texIdx = aTexIdx;
    markDirty();
}

void textureComponent::setShaderIdx(GLuint aShaderIdx) {
    shaderIdx = aShaderIdx;
    markDirty();
}

void textureComponent::bind() const {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texIdx);
    glUseProgram(shaderIdx);
}

void textureComponent::unbind() const {
    glUseProgram(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}
