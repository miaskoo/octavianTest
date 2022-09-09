#include "label.h"
#include "transformComponent.h"
#include "labelComponent.h"
#include "colorComponent.h"

#ifdef MACOS
#include "GLUT/glut.h"
#else
#include "glew/glew.h"
#include "freeglut/freeglut.h"
#endif


label::label() {
    auto component = addComponent<labelComponent>();
    component->setFont(GLUT_BITMAP_HELVETICA_18);
    addComponent<colorComponent>();
}

void label::createCash() {
    for (size_t n = 0U; n < cashArray.size(); n++) {
        cashArray[n] = std::make_shared<labelCash>();
    }
}

vec2f label::calcAutoSize(const std::string& text, void* font) const {
    if (font == NULL) {
        return {};
    }
    vec2f result;
    for (auto& c : text) {
        result[0] += glutBitmapWidth(font, c);
    }
    result[1] = 18;
    return result;
}

void label::updateCash() {
    auto busyLabelComponent = getBusyCash()->getComponent<labelComponent>();
    auto busyTextLen = busyLabelComponent->getText().length();
    auto currentLabelComponent = getComponent<labelComponent>();
    auto currentTextLen = currentLabelComponent->getText().length();
    
    auto currentTransformComponent = getTransformComponent();

    if (busyTextLen != currentTextLen) {
        auto newSize = calcAutoSize(currentLabelComponent->getText(), currentLabelComponent->getFont());
        currentTransformComponent->setSize(newSize);
    }
    currentTransformComponent->updateCashTransform(wThis);
    auto freeTransformComponent = getFreeCash()->getComponent<transformComponent>();
    copyComponent(freeTransformComponent);
    freeTransformComponent->setCashSize(currentTransformComponent->getScale());
    copyComponent(getFreeCash()->getComponent<labelComponent>());
    copyComponent(getFreeCash()->getComponent<colorComponent>());
}

label::labelCash::labelCash() {
    addComponent<labelComponent>();
    addComponent<colorComponent>();
}
