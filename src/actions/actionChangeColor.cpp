#include "actionChangeColor.h"
#include "colorComponent.h"
#include "entity.h"

actionChangeColor::actionChangeColor(vec4f aStartColor, vec4f aNewColor, int aTime, std::function<void()> aCallback) :
actionBase(aCallback),
startColor(aStartColor),
newColor(aNewColor),
time(0),
fullTime(aTime) {}


void actionChangeColor::update(std::weak_ptr<entity> object, float dt)  {
    if (isEnd()) {
        return;
    }
    time += dt;
    float percent = time / fullTime;
    auto percentColor = newColor - startColor;
    for (int n = 0; n < percentColor.size(); n++) {
        percentColor[n] = startColor[n] + percentColor[n] * percent;
    }
    if (auto pObject = object.lock()) {
        pObject->getComponent<colorComponent>()->setColor(percentColor.x(), percentColor.y(), percentColor.z(), percentColor.w());
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
