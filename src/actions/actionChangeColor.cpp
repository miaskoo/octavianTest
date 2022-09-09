#include "actionChangeColor.h"
#include "colorComponent.h"
#include "entity.h"

actionChangeColor::actionChangeColor(vec4f aStartColor, vec4f aNewColor, int aTime, std::function<void()> aCallback) :
actionBase(aCallback),
startColor(aStartColor),
newColor(aNewColor),
time(0),
fullTime(aTime) {
    for (size_t n = 0U; n < 4U; n++) {
        if (newColor[n] > startColor[n]) {
            stepColor[n] = newColor[n] - startColor[n];
        }
        else {
            stepColor[n] = startColor[n] - newColor[n];
        }
    }
}


void actionChangeColor::update(std::weak_ptr<entity> object, float dt)  {
    if (isEnd()) {
        return;
    }
    time += dt;
    if (time > fullTime) {
        end(object);
        return;
    }
    float percent = time / fullTime;
    
    vec4f resultColor;
    for (int n = 0; n < 4U; n++) {
        if (newColor[n] > startColor[n]) {
            resultColor[n] = startColor[n] + stepColor[n] * percent;
        }
        else {
            resultColor[n] = startColor[n] - stepColor[n] * percent;
        }
    }
    if (auto pObject = object.lock()) {
        pObject->getComponent<colorComponent>()->setColor(resultColor.x(), resultColor.y(), resultColor.z(), resultColor.w());
    }
}

bool actionChangeColor::isEnd() const {
    return time > fullTime;
}

void actionChangeColor::end(std::weak_ptr<entity> object) {
    if (auto pObject = object.lock()) {
        pObject->getComponent<colorComponent>()->setColor(newColor.x(), newColor.y(), newColor.z(), newColor.w());
    }
    if (callback) {
        callback();
    }
}

void actionChangeColor::reset() {
    time = 0.f;
}
