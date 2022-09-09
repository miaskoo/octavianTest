#include "node.h"
#include "transformComponent.h"

node::node() : entity(dimension::TWO) {
    addComponent<transformComponentMain<vec2f>>();
}

void node::updateCash() {
    getTransformComponent()->updateCashTransform(wThis);
    if (auto cash = getFreeCash()) {
        copyComponent(cash->getComponent<transformComponent>());
    }
}

node::nodeCash::nodeCash() {
    addComponent<transformComponent>();
}
