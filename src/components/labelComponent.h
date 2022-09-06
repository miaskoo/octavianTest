#pragma once

#include "componentBase.h"
#include <string>

class labelComponent : public componentBase<componentId::LABEL> {
public:
    labelComponent() = default;
    ~labelComponent() = default;
    virtual void bind() const override {};
    virtual void unbind() const override {};
    virtual void use() const override;
    void setText(const std::string& aText);
    void setFont(void* aFont);
    const std::string& getText() const;
    void* getFont() const;
private:
    std::string text;
    void* font = nullptr;
};
