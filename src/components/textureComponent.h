#pragma once

#include "componentBase.h"
#include "GLUT/glut.h"

class textureComponent : public componentBase<componentId::TEXTURE> {
public:
    textureComponent() = default;
    ~textureComponent() = default;
    void setTexIdx(GLuint aTexIdx);
    void setShaderIdx(GLuint aShaderIdx);
    virtual void bind() const override;
    virtual void unbind() const override;
    virtual void use() const override {};
protected:
    GLuint texIdx = 0;
    GLuint shaderIdx = 0;
};
