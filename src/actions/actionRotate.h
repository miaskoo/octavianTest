#pragma once
#include "actionBase.h"
#include "struct.h"

class entity;

class actionRotate : public actionBase {
public:
    actionRotate() = delete;
    actionRotate(vec3f aAxis, float angle, int aTime, quaternion aStartRotate, std::function<void()> aCallback = nullptr);
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
