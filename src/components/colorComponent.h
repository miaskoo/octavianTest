#pragma once

#include "componentBase.h"
#include "struct.h"

class colorComponent : public componentBase<componentId::COLOR> {
public:
    colorComponent() = default;
    ~colorComponent() = default;
    void setAlphaMode(bool value);
    void setColor(float r, float g, float b);
    void setColor(float r, float g, float b, float a);
    virtual void bind() const override;
    virtual void unbind() const override;
    virtual void use() const override;
private:
    vec4f color = {1.f, 1.f, 1.f, 1.f};
    bool alphaMode = true;
};
