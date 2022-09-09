#pragma once
#include "actionBase.h"
#include "struct.h"

class entity;

class actionRotate : public actionBase {
public:
    actionRotate() = delete;
    actionRotate(quaternion aStartRotate, vec3f aAxis, float aAngle, int aTime, std::function<void()> aCallback = nullptr);
    ~actionRotate() = default;
    
    virtual void update(std::weak_ptr<entity>object, float dt) override;
    virtual void end(std::weak_ptr<entity>object) override;
    virtual bool isEnd() const override;
    virtual void reset() override;
private:
    const float fullTime;
    float time;
    int destination;
    vec3f axis;
    quaternion startRotate;
};
