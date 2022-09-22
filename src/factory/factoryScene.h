#pragma once

#include "factoryEntity.h"
#include <memory>

class scene;

class factoryScene {
public:
    factoryScene() = default;
    ~factoryScene() = default;
    std::shared_ptr<scene> createSlotMachineScene();
    factoryEntity fEntity;
};
