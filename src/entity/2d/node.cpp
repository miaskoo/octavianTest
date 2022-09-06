#include "node.h"
#include "transformComponent.h"

node::node() : entity(dimension::TWO) {
    addComponent<transformComponentMain<vec2f>>();
}

void node::updateCash() {
    getComponent<transformComponent>()->updateCashTransform(wThis);
    if (auto cash = getFreeCash()) {
        copyComponent(cash->getComponent<transformComponent>());
    }
}

node::nodeCash::nodeCash() {
    addComponent<transformComponentEmptyInterface>();
}
