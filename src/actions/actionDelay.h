#pragma once

#include "actionBase.h"

class actionDelay : public actionBase {
public:
    actionDelay() = delete;
    actionDelay(int aTime, std::function<void()> aCallback = nullptr);
    ~actionDelay() = default;
    virtual void update(std::weak_ptr<entity> object, float dt) override;
    virtual void end(std::weak_ptr<entity>object) override;
    virtual bool isEnd() const override;
    virtual void reset() override;
private:
    const float fullTime;
    float time;
};
