#pragma once

#include <unordered_map>
#include <string>
#include <GLUT/glut.h>

class factoryTexture {
public:
    factoryTexture() = default;
    ~factoryTexture() = default;
    void loadTexturs();
    GLuint getTextureIdx(const std::string& dir);
    GLuint getShaderTextureIdx();
private:
    std::unordered_map<std::string, GLuint> nameToTex;
    GLuint shaderTextureProgram;
};
