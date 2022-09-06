#pragma once

#include "componentBase.h"

class bufferComponent : public componentBase<componentId::BUFFER> {
public:
    bufferComponent() = default;
    ~bufferComponent() = default;
    void setBuffIdx(GLuint aBuffIdx);
    void setBuffSize(GLuint aBuffSize);
    virtual void bind() const override;
    virtual void unbind() const override;
    virtual void use() const override;
    void setCullface(bool value) const;
private:
    GLuint buffIdx = 0;
    GLuint buffSize = 0;
};

