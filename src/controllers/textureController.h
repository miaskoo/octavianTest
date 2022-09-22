#pragma once

#include <string>
#include <unordered_map>

class textureController {
public:
    textureController() = default;
    ~textureController() = default;
    
    void init();
    unsigned int getTextureIdx(const std::string& dir);
    unsigned int getDefaultShaderProgramIdx();
private:
    bool inited = false;
    
    std::unordered_map<std::string, unsigned int> nameToTex;
    unsigned int defaultShaderProgramIdx = 0U;
};
