#pragma once

#include "componentBase.h"
#include "struct.h"

class colorComponent : public componentBase<componentId::COLOR> {
public:
    colorComponent() = default;
    ~colorComponent() = default;
    void setAlphaMode(bool value);
    void setColor(unsigned int r, unsigned int g, unsigned int b);
    void setColor(unsigned int r, unsigned int g, unsigned int b, unsigned int a);
    unsigned int* getColor();
    bool isAlphaMode();
private:
    unsigned int color[4] = {255, 255, 255, 255};
    bool alphaMode = true;
};
