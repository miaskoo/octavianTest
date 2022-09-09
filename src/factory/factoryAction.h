#pragma once

#include <functional>
#include "struct.h"

class actionBase;

class factoryAction {
public:
    factoryAction() = default;
    ~factoryAction() = default;
    actionBase* createRotateAction(quaternion startRotate, vec3f axis, float angle, int time, std::function<void()> callback = nullptr);
    actionBase* createDelayAction(int time, std::function<void()> callback = nullptr);
    actionBase* createRepeatAction(actionBase* action, int count, std::function<void()> callback = nullptr);
    actionBase* createRepeatInfinityAction(actionBase* action, std::function<void()> callback = nullptr);
    actionBase* createChangeColorAction(vec4f startColor, vec4f endColor, int time, std::function<void()> callback = nullptr);
    actionBase* createActionSequence(std::vector<actionBase*> actions, std::function<void()> callback = nullptr);
};
