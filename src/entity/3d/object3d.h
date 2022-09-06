#pragma once
#include "entity.h"

class object3d : public entity {
public:
    object3d();
    ~object3d() = default;
    virtual void createCash() override;
protected:
    virtual void updateCash() override;
    
    class object3dCash : public entityCash {
    public:
        object3dCash();
        ~object3dCash() = default;
    };
};


