#pragma once

#include "textureComponent.h"

class spriteComponent : public textureComponent {
public:
    spriteComponent() = default;
    ~spriteComponent() = default;
    virtual void use() const override;
};
