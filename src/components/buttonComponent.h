#pragma once
#include "spriteComponent.h"

class buttonComponent : public spriteComponent {
public:
    buttonComponent() = default;
    ~buttonComponent() = default;
    virtual void bind() const override;
    void setTexIdx(GLuint aTexIdxWait, GLuint aTexIdxHover, GLuint aTexIdxPress);
    stateButton getState();
    void setState(stateButton aState);
private:
    stateButton state = stateButton::WAIT;
    GLuint texIdx[3];
};
