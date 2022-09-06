#pragma once
#include "entity.h"
#include <list>

class scene : public entity {
public:
    scene();
    ~scene() = default;
    virtual void createCash() override {}
    void updateDirtyCash(entity* obj);
    void createNewCash(entity* obj, std::list<std::shared_ptr<entityCash>>& cashList);
    void checkDirty(entity* obj, bool& result);
protected:
    virtual void updateCash() override {}
};
