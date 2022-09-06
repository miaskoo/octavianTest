#include "actionDelay.h"
#include "entity.h"

actionDelay::actionDelay(int aTime, std::function<void()> aCallback) :
actionBase(aCallback),
fullTime(aTime),
time(0) {}

void actionDelay::update(std::weak_ptr<entity> object, float dt)  {
    if (isEnd()) {
        return;
    }
    time += dt;
}

bool actionDelay::isEnd() const {
    return time > fullTime;
}

void actionDelay::end(std::weak_ptr<entity> object) {
    if (callback) {
        callback();
    }
}

void actionDelay::reset() {
    time = 0.f;
}
