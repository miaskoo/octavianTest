#include "factoryAction.h"
#include "actionRotate.h"
#include "actionDelay.h"
#include "actionRepeat.h"
#include "actionChangeColor.h"
#include "actionSequence.h"

actionBase* factoryAction::createRotateAction(quaternion startRotate, vec3f axis, float angle, int time, std::function<void()> callback) {
    return new actionRotate(startRotate, axis, angle, time, callback);
}

actionBase* factoryAction::createDelayAction(int time, std::function<void()> callback) {
    return new actionDelay(time, callback);
}

actionBase* createRepeatAction(actionBase* action, int count, std::function<void()> callback) {
    if (!action) {
        return nullptr;
    }
    return new actionRepeat(action, count, callback);
}

actionBase* factoryAction::createRepeatInfinityAction(actionBase* action, std::function<void()> callback) {
    if (!action) {
        return nullptr;
    }
    return new actionRepeat(action, callback);
}

actionBase* factoryAction::createChangeColorAction(vec4f startColor, vec4f endColor, int time, std::function<void()> callback) {
    return new actionChangeColor(startColor, endColor, time, callback);
}

actionBase* factoryAction::createActionSequence(std::vector<actionBase *> actions, std::function<void()> callback) {
    auto actionSeq = new actionSequence(callback);
    for (auto& action : actions) {
        if (action) {
            actionSeq->addAction(action);
        }
    }
    return actionSeq;
}
