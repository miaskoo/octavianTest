#include "actionRotate.h"
#include "transformComponent.h"

actionRotate::actionRotate(vec3f aAxis, float angle, int aTime, quaternion aStartRotate, std::function<void()> aCallback) :
actionBase(aCallback),
destination(angle),
axis(aAxis),
fullTime(aTime),
time(0),
startRotate(aStartRotate) {}

bool actionRotate::isEnd() const {
    return time > fullTime;
}

void actionRotate::update(std::weak_ptr<entity> object, float dt)  {
    time += dt;
    if (isEnd()) {
        return;
    }
    auto percent = time / fullTime;
    if (auto pObject = object.lock()) {
        pObject->getComponent<transformComponent>()->setRotate(startRotate * quaternion::getFromEuler(axis, destination * percent));
    }
}

void actionRotate::end(std::weak_ptr<entity> object) {
    if (auto pObject = object.lock()) {
        pObject->getComponent<transformComponent>()->setRotate(startRotate * quaternion::getFromEuler(axis, destination));
    }
    if (callback) {
     callback();
    }
}

void actionRotate::reset() {
    time = 0.f;
    startRotate = startRotate * quaternion::getFromEuler(axis, destination);
}