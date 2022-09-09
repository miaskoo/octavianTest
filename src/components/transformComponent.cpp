#include "transformComponent.h"
#include "entity.h"

#ifdef MACOS
#include "GLUT/glut.h"
#else
#include "glew/glew.h"
#include "freeglut/freeglut.h"
#endif


transformComponent::transformComponent() {
    for (size_t n = 0; n < 16; n++) {
        cashRotate[n] = 0.0f;
    }
}

vec3f transformComponent::getCashPos() const {
    return cashPos;
}

vec3f transformComponent::getCashSize() const {
    return cashSize;
}

float const* transformComponent::getCashRotate() const {
    return cashRotate;
}

void transformComponent::setCashPos(vec3f aPos) {
    cashPos = aPos;
}

void transformComponent::setCashSize(vec3f aSize) {
    cashSize = aSize;
}

void transformComponent::setCashRotate(float *aRotate) {
    for (size_t n = 0; n < 16; n++) {
        cashRotate[n] = aRotate[n];
    }
}

void transformComponent::bind() const {
    glPushMatrix();
    glTranslatef(cashPos[0], cashPos[1], cashPos[2]);
    glMultMatrixf(cashRotate);
    glScalef(cashSize[0], cashSize[1], cashSize[2]);
}

void transformComponent::unbind() const {
    glPopMatrix();
}


