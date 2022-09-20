#include "button.h"
#include "transformComponent.h"
#include "colorComponent.h"
#include "clickComponent.h"

button::button() {
    addComponent<colorComponent>();
    //addComponent<clickComponent>();
}

void button::updateCash(size_t freeCashIdx, size_t busyCashIdx) {
    node::updateCash(freeCashIdx, busyCashIdx);
    copyComponent(getCash(freeCashIdx)->getComponent<colorComponent>());
    //copyComponent(getFreeCash()->getComponent<buttonComponent>());
}

void button::createCash() {
    for (size_t n = 0U; n < cashArray.size(); n++) {
        cashArray[n] = std::make_shared<buttonCash>();
    }
}

void button::updateWithMousePos(int x, int y, stateMouse state) {
//    auto bComponent = getComponent<clickComponent>();
//    auto prevState = bComponent->getState();
    auto newState = stateButton::WAIT;
    auto component = getComponent<transformComponent>();
    vec3f pos = component->getCashPos();
    vec3f size = component->getCashSize();
    auto x1 = pos[0];
    auto y1 = pos[1];
    auto x2 = pos[0] + size[0];
    auto y2 = pos[1] + size[1];
    if (x1 > x2) {
        std::swap(x1, x2);
    }
    if (y1 > y2) {
        std::swap(y1, y2);
    }
    if (state == stateMouse::CLICK_OUT) {
        isClicked = false;
    }
//    if (bComponent->isClickable()) {
//        if (x > x1 && x < x2 && y > y1 && y < y2) {
//            if (state == stateMouse::CLICK) {
//                newState = stateButton::PRESS;
//                if (!isClicked && prevState != newState) {
//                    isClicked = true;
//                    //bComponent->use();
//                }
//            }
//            else {
//                newState = stateButton::HOVER;
//            }
//        }
//    }
//    else {
//        newState = stateButton::LOCK;
//    }
    
//    if (prevState != newState) {
//        //bComponent->setState(newState);
//        markDirty();
//    }
    if (newState != stateButton::PRESS) {
        entity::updateWithMousePos(x, y, state);
    }
}

button::buttonCash::buttonCash() {
    addComponent<colorComponent>();
    //addComponent<buttonComponent>();
}
