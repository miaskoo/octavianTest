#include "sprite.h"
#include "colorComponent.h"
#include "textureComponent.h"
#include "spriteComponent.h"

sprite::sprite() {
    addComponent<colorComponent>();
    addComponent<spriteComponent>();
}


void sprite::updateCash() {
    node::updateCash();
    copyComponent(getFreeCash()->getComponent<colorComponent>());
    copyComponent(getFreeCash()->getComponent<spriteComponent>());
}


void sprite::createCash() {
    for (size_t n = 0U; n < cashArray.size(); n++) {
        cashArray[n] = std::make_shared<spriteCash>();
    }
}

sprite::spriteCash::spriteCash() {
    addComponent<colorComponent>();
    addComponent<spriteComponent>();
}
