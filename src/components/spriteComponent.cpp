#include "spriteComponent.h"
#include "glFunctional.h"

void spriteComponent::use() const {
    glForwarder::drawTextureQuad(1);
}
