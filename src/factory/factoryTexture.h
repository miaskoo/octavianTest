#pragma once

#include <unordered_map>
#include <string>
#ifdef MACOS
#include "GLUT/glut.h"
#else
#include "glew/glew.h"
#include "freeglut/freeglut.h"
#endif

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
