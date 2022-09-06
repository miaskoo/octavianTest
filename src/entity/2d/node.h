#pragma once
#include "entity.h"
#include <vector>
#include <memory>

class node : public entity {
public:
    node();
    ~node() = default;
protected:
    virtual void updateCash() override;
    
    class nodeCash : public entityCash {
    public:
        ~nodeCash() = default;
    protected:
        nodeCash();
    };
private:
};
