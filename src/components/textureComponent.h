#pragma once

#include "componentBase.h"

class textureComponent : public componentBase<componentId::TEXTURE> {
public:
    textureComponent() = default;
    ~textureComponent() = default;
    void setTexIdx(unsigned int aTexIdx);
    void setShaderIdx(unsigned int aShaderIdx);
    virtual void bind() const override;
    virtual void unbind() const override;
    virtual void use() const override {};
protected:
    unsigned int texIdx = 0;
    unsigned int shaderIdx = 0;
};
