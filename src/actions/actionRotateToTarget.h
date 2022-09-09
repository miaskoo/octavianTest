#pragma once

#include "struct.h"
#include "actionBase.h"
#include <functional>

class actionRotateToTarget : actionBase {
public:
    actionRotateToTarget(quaternion aStartRotate, quaternion aTargetRotate, float time, std::function<void()> aCallback);
    ~actionRotateToTarget();
private:
    quaternion startRotate;
    quaternion targetRotate;
};
