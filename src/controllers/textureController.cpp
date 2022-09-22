#include "textureController.h"
#include "factoryTexture.h"


unsigned int textureController::getTextureIdx(const std::string& dir) {
    auto iter = nameToTex.find(dir);
    if (iter != nameToTex.end()) {
        return iter->second;
    }
    return 0;
}

unsigned int textureController::getDefaultShaderProgramIdx() {
    return defaultShaderProgramIdx;
}

void textureController::init() {
    if (inited) {
        return;
    }
    std::string names[] = { "slot.png", "slotTorus.png", "scene.png", "wait.png", "cover.png", "press.png" };
    for (const auto& name : names) {
        nameToTex[name] = factoryTexture::createTexture(name);
    }
    
    for (const auto& texIdx : nameToTex) {
        if (texIdx.second == 0) {
            assert(false && (texIdx.first + " texture not load").c_str());
        }
    }
    
    defaultShaderProgramIdx = factoryTexture::createDefaultShaderProgram();
    
    inited = true;
}
