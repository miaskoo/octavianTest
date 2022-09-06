#include "factoryTexture.h"
#include "../include/SOIL/SOIL.h"
#include <iostream>
#include <filesystem>
#include <assert.h>
#include <freeglut/freeglut.h>

void factoryTexture::loadTexturs() {
#ifdef MACOS
    const std::string dirSymbols = "/";
#else
    const std::string dirSymbols = "\\";
#endif
    const std::string dirResource = dirSymbols + "resource" + dirSymbols;
    std::string names[] = { "slot.png", "slotTorus.png", "scene.png", "wait.png", "hover.png", "press.png" };
    for (const auto& name : names) {
        auto buildPatch = std::filesystem::absolute("." + dirSymbols);
        auto projectPatch = buildPatch.parent_path().parent_path().parent_path();
        auto needDir = projectPatch.string();
        needDir += dirResource + name;
        nameToTex[name] = SOIL_load_OGL_texture(needDir.c_str(), SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, 0);
    }
    
    for (const auto& texIdx : nameToTex) {
        if (texIdx.second == 0) {
            assert(false && (texIdx.first + " texture not load").c_str());
        }
    }
    const GLchar* vertexShaderSource = {
    "\
       attribute vec3 inPosition;\
       attribute vec2 inTexcoord;\
       varying vec2 texCoord;\
       void main()\
       {\
           gl_Position = gl_ModelViewProjectionMatrix * vec4(inPosition.xyz,1);\
           gl_TexCoord[0].st = inTexcoord;\
           texCoord = inTexcoord;\
       }"
    };

    const GLchar* fragmentShaderSource = {
    "\
        uniform sampler2D mytex;\
        varying vec2 texCoord;\
        void main()\
        {\
            gl_FragColor = texture2D(mytex,gl_TexCoord[0].st);\
        }"
    };
    
    GLint success;
    GLchar infoLog[512] {0};

    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    std::cout << infoLog << std::endl;
    
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);

    std::cout << infoLog << std::endl;

    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);


    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);

    std::cout << infoLog << std::endl;

    shaderTextureProgram = glCreateProgram();

    glAttachShader(shaderTextureProgram, vertexShader);
    glAttachShader(shaderTextureProgram, fragmentShader);
    glLinkProgram(shaderTextureProgram);
    glValidateProgram(shaderTextureProgram);
    
    glGetProgramInfoLog(shaderTextureProgram, 512, NULL, infoLog);
      
    std::cout << infoLog << std::endl;
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

GLuint factoryTexture::getTextureIdx(const std::string& dir) {
    auto iter = nameToTex.find(dir);
    if (iter != nameToTex.end()) {
        return iter->second;
    }
    return 0;
}

GLuint factoryTexture::getShaderTextureIdx() {
    return shaderTextureProgram;
}
