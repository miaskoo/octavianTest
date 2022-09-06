#pragma once
#include <string>
#include <memory>
#include "struct.h"
#include "factoryBuffer.h"

class entity;

#pragma once

class factoryEntity {
public:
    factoryEntity() = default;
    ~factoryEntity() = default;
    std::shared_ptr<entity> createNode(vec2f pos = {}, vec2f size = {});
    std::shared_ptr<entity> createLabel(vec2f pos = {}, const std::string& text = "", void* font = nullptr);
    std::shared_ptr<entity> createQuadrilateral(vec3f pos, vec3f size, quaternion rotate);
    std::shared_ptr<entity> createTorus(vec3f pos, vec3f size, int countSector, quaternion q = quaternion());
    std::shared_ptr<entity> createSprite(vec2f pos = {}, vec2f size = {});
    std::shared_ptr<entity> createButton(vec2f pos = {}, vec2f size = {});
private:
    factoryBuffer fBuffer;
};
