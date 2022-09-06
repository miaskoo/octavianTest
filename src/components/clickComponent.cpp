#include "clickComponent.h"

void clickComponent::use() const {
    if (clickCallback) {
        clickCallback();
    }
}

void clickComponent::setClickCallback(std::function<void()>&& aClickCallback) {
    clickCallback = std::move(aClickCallback);
}

bool clickComponent::isClickable() {
    return clickable;
}

void clickComponent::setClickable(bool value) {
    clickable = value;
}
