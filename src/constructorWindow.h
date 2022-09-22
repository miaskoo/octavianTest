#pragma once
#include "factoryScene.h"
#include "struct.h"
#include <atomic>
#include "textureController.h"
#include "bufferController.h"

class scene;
class entity;

class constructorWindow {
public:
    static constructorWindow* getInstance();
    static void cleanup();
    
    void initWindow(int argc, char **argv);
    void initResource();
    void initGlutFunc();
    
    void setGameScene(std::shared_ptr<scene> aScene);
    void createInfoNode();
    void destroyInfoNode();
    void startUpdateThread();
   
    void startMainLoop();
    
    int getScreenW();
    int getScreenH();
    
    friend void glutDisplayFuncForwarder();
    friend void glutIdleFuncForwarder();
    friend void glutMouseFuncForwarder(int button, int state, int x, int y);
    friend void glutPassiveMotionFuncForwarder(int x, int y);
    
    bufferController* getBufferController();
    textureController* getTextureController();
private:
    constructorWindow() = default;
    ~constructorWindow() = default;
    
    void updateWindow();
    void updateScene(float dt);
    
    void renderScene();
    void idleScene();
    
    void clickWindow(int button, int state, int x, int y);
    void setMousePos(int x, int y);
    
    size_t getCashIdx(typeCash type);
    
    const int screenW = 1024;
    const int screenH = 900;
    
    int currentWindow = 0;
    
    int lockFps = 60;
    int minDtFpsMs = 1000 / lockFps;
    int minDtUpdateMs = 25;
    
    long long lastIdleTime = 0;
    
    int lastFps = 0;
    int currentFps = 0;
    int lastTimeCheckFps = 0;
    
    std::atomic_int mouseX = 0;
    std::atomic_int mouseY = 0;
    std::atomic<stateMouse> click = stateMouse::IDLE;
    
    std::shared_ptr<scene> mainScene;
    
    std::shared_ptr<entity> uiNode;
    std::shared_ptr<entity> fpsLabel;
    std::shared_ptr<entity> timeLabel;
    std::shared_ptr<entity> mousePosLabel;
    
    std::atomic_bool switchCash = false;
    std::atomic_bool cashDirty = false;
    
    bufferController cBuffer;
    textureController cTexture;
    
    static inline constructorWindow* instance = nullptr;
};
