#pragma once

#include <unordered_map>
#include <string>

class factoryTexture {
public:
    factoryTexture() = default;
    ~factoryTexture() = default;
    void loadTexturs();
    unsigned int getTextureIdx(const std::string& dir);
    unsigned int getShaderTextureIdx();
private:
    std::unordered_map<std::string, unsigned int> nameToTex;
    unsigned int shaderTextureProgram;
};
