#include "object3d.h"
#include "math.h"
#include "transformComponent.h"
#include "bufferComponent.h"
#include "textureComponent.h"

object3d::object3d() : entity(dimension::THREE) {
    addComponent<transformComponentMain<vec3f>>();
    addComponent<bufferComponent>();
    addComponent<textureComponent>();
}

void object3d::createCash() {
    for (size_t n = 0U; n < cashArray.size(); n++) {
        cashArray[n] = std::make_shared<object3dCash>();
    }
}

void object3d::updateCash() {
    getComponent<transformComponent>()->updateCashTransform(wThis);
    
    copyComponent(getFreeCash()->getComponent<transformComponent>());
    copyComponent(getFreeCash()->getComponent<bufferComponent>());
    copyComponent(getFreeCash()->getComponent<textureComponent>());
}

object3d::object3dCash::object3dCash() {
    addComponent<transformComponentEmptyInterface>();
    addComponent<bufferComponent>();
    addComponent<textureComponent>();
}
