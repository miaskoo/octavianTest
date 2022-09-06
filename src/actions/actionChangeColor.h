#pragma once

#include "actionBase.h"
#include "struct.h"

class actionChangeColor : public actionBase {
public:
    actionChangeColor() = delete;
    actionChangeColor(vec4f startColor, vec4f newColor, int time, std::function<void()> aCallback = nullptr);
    ~actionChangeColor() = default;
    
    virtual void update(std::weak_ptr<entity>object, float dt) override;
    virtual void end(std::weak_ptr<entity>object) override;
    virtual bool isEnd() const override;
    virtual void reset() override;
private:
    vec4f startColor;
    vec4f newColor;
    
    const float fullTime;
    float time;
};
