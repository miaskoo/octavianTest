#pragma once

#include "componentBase.h"

class bufferComponent : public componentBase<componentId::BUFFER> {
public:
    bufferComponent() = default;
    ~bufferComponent() = default;
    void setBuffIdx(unsigned int aBuffIdx);
    void setBuffSize(unsigned int aBuffSize);
    virtual void bind() const override;
    virtual void unbind() const override;
    virtual void use() const override;
    void setCullface(bool value) const;
private:
    unsigned int buffIdx = 0;
    unsigned int buffSize = 0;
};

