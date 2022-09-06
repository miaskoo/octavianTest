#pragma once

#include "node.h"

class sprite : public node {
public:
    sprite();
    ~sprite() = default;
    virtual void createCash() override;
    virtual void updateCash() override;
protected:
    class spriteCash : public nodeCash {
    public:
        spriteCash();
        ~spriteCash() = default;
    };
};
