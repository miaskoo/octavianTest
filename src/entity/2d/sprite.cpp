#include "sprite.h"
#include "colorComponent.h"
#include "textureComponent.h"
#include "renderComponent.h"
#include "systemRender.h"

sprite::sprite() {
    addComponent<colorComponent>();
    addComponent<textureComponent>();
    addComponent<spriteComponent>();
    systemRender::getInstance()->registerEntity(this);
}


void sprite::updateCash(size_t freeCashIdx, size_t busyCashIdx) {
    node::updateCash(freeCashIdx, busyCashIdx);
    copyComponent(getCash(freeCashIdx)->getComponent<colorComponent>());
    copyComponent(getCash(freeCashIdx)->getComponent<spriteComponent>());
}


void sprite::createCash() {
    for (size_t n = 0U; n < cashArray.size(); n++) {
        cashArray[n] = std::make_shared<spriteCash>();
    }
}

sprite::spriteCash::spriteCash() {
    addComponent<colorComponent>();
    addComponent<textureComponent>();
    addComponent<spriteComponent>();
}
