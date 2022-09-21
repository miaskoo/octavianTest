#pragma once
#include <string>
#include <memory>
#include "struct.h"
#include "factoryBuffer.h"
#include "factoryTexture.h"

class entity;

#pragma once

class factoryEntity {
public:
    factoryEntity() = default;
    ~factoryEntity() = default;
    std::shared_ptr<entity> createNode();
    std::shared_ptr<entity> createLabel(const std::string& text = "");
    std::shared_ptr<entity> createQuadrilateral();
    std::shared_ptr<entity> createTorus(size_t countSector);
    std::shared_ptr<entity> createSprite(const std::string& dirTexture = "");
    std::shared_ptr<entity> createButton(const std::string& dirTextureNormal = "", const std::string& dirTextureCover = "", const std::string& dirTextureClick = "");
    
    factoryTexture& getTextureFactory();
private:
    factoryBuffer fBuffer;
    factoryTexture fTexture;
};
