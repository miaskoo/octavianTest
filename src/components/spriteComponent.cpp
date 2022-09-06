#include "spriteComponent.h"
#include "GLUT/glut.h"

void spriteComponent::use() const {
    glBegin(GL_QUADS);
    glTexCoord2i(0,0);
    glVertex2i(0,0);
    glTexCoord2i(1,0);
    glVertex2i(1,0);
    glTexCoord2i(1,1);
    glVertex2i(1,1);
    glTexCoord2i(0,1);
    glVertex2i(0,1);
    glEnd();
}
