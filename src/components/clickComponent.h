#pragma once

#include "buttonComponent.h"
#include <functional>

class clickComponent : public buttonComponent {
public:
    clickComponent() = default;
    ~clickComponent() = default;
    virtual void use() const override;
    void setClickCallback(std::function<void()>&& aClickCallback);
    void setClickable(bool value);
    bool isClickable();
private:
    std::function<void()> clickCallback;
    bool clickable = true;
};
