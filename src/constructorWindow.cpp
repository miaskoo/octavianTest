#include "constructorWindow.h"

#include "renderComponent.h"
#include "transformComponent.h"
#include "scene.h"

#include "renderSystem.h"
#include "mouseSystem.h"

#include "factoryScene.h"

#include <stdio.h>
#include <chrono>
#include <ctime>
#include <thread>

#ifdef MACOS
#include "GLUT/glut.h"
#else
#include "glew/glew.h"
#include "freeglut/freeglut.h"
#endif

void constructorWindow::initWindow(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(0,0);
    glutInitWindowSize(screenW,screenH);
    renderSystem::getInstance()->setWindowSize(screenW, screenH);
    srand(time(0U));
    glutCreateWindow("test");
#ifndef MACOS
    glewInit();
#endif
}

void constructorWindow::initResource() {
    fScene.fEntity.getTextureFactory().loadTexturs();
}

void constructorWindow::updateWindow() {
    long long time = 0;
    float dt = 0.0f;
    
    while(true) {
        auto lastTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    
        if (fpsLabel) {
            fpsLabel->getComponent<labelComponent>()->setText("Fps: " + std::to_string(lastFps));
        }
        if (timeLabel) {
            timeLabel->getComponent<labelComponent>()->setText("Time: " + std::to_string(time / 1000.f));
        }
        if (mousePosLabel) {
            mousePosLabel->getComponent<labelComponent>()->setText("X: " + std::to_string(mouseX.load()) + " Y: " + std::to_string(mouseY.load()));
        }
        if (dt > 0.f) {
            updateScene(dt);
        }
        
        auto currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        dt = currentTime - lastTime;
        if (dt < minDtUpdateMs) {
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<long long>(minDtUpdateMs - dt)));
            dt = minDtUpdateMs;
        }
        time+= dt;
    }
}

void constructorWindow::updateScene(float dt) {
    auto clickState = click.load();
    mouseSystem::getInstance()->update(mouseX.load(), mouseY.load(), clickState);
    if (clickState == stateMouse::CLICK_OUT) {
        click.store(stateMouse::IDLE);
    }
    mainScene->update(dt);
    if (cashDirty.load()) {
        return;
    }
    bool dirty = false;
    mainScene->checkDirty(mainScene.get(), dirty);
    if (dirty) {
        mainScene->updateCash(getCashIdx(typeCash::FREE), getCashIdx(typeCash::BUSY));
        switchCash.store(!switchCash.load());
        cashDirty.store(true);
    }
}

size_t constructorWindow::getCashIdx(typeCash type) {
    if (switchCash.load()) {
        if (type == typeCash::FREE) {
            return static_cast<size_t>(typeCash::BUSY);
        }
        return static_cast<size_t>(typeCash::FREE);
    }
    return static_cast<size_t>(type);
}

void constructorWindow::renderScene() {
    if (currentFps < lockFps) {
        if (cashDirty.load()) {
            cashDirty.store(false);
        }
        renderSystem::getInstance()->update(getCashIdx(typeCash::BUSY));
        currentFps++;
    }
}

void constructorWindow::idleScene() {
    auto currentIdleTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    auto dt = currentIdleTime - lastIdleTime;
    if (dt < minDtFpsMs) {
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<long long>(minDtFpsMs - dt)));
        dt = minDtFpsMs;
    }
    
    lastTimeCheckFps += dt;
    if (lastTimeCheckFps >= 1000) {
        lastTimeCheckFps = 0;
        lastFps = currentFps;
        currentFps = 0;
    }
    glutPostWindowRedisplay(currentWindow);
    lastIdleTime = currentIdleTime;
}

void constructorWindow::clickWindow(int button, int state, int x, int y) {
    constructorWindow::getInstance()->setMousePos(x, y);
    click.store(state == 0 ? stateMouse::CLICK : stateMouse::CLICK_OUT);
}

void glutPassiveMotionFuncForwarder(int x, int y) {
    constructorWindow::getInstance()->setMousePos(x, y);
}

void constructorWindow::startUpdateThread() {
    std::thread updateScene([](){
        getInstance()->updateWindow();
    });
    updateScene.detach();
}

void constructorWindow::startMainLoop() {
    glutMainLoop();
}

void constructorWindow::setMousePos(int x, int y) {
    mouseX.store(x);
    mouseY.store(y);
}

void glutDisplayFuncForwarder() {
    constructorWindow::getInstance()->renderScene();
}

void glutIdleFuncForwarder() {
    constructorWindow::getInstance()->idleScene();
}

void glutMouseFuncForwarder(int button, int state, int x, int y) {
    constructorWindow::getInstance()->clickWindow(button, state, x, y);
}

void constructorWindow::initGlutFunc() {
    glutDisplayFunc(glutDisplayFuncForwarder);
    glutIdleFunc(glutIdleFuncForwarder);
    glutMouseFunc(glutMouseFuncForwarder);
    glutPassiveMotionFunc(glutPassiveMotionFuncForwarder);
    currentWindow = glutGetWindow();
}

int constructorWindow::getScreenW() {
    return screenW;
}

int constructorWindow::getScreenH() {
    return screenH;
}

constructorWindow* constructorWindow::getInstance() {
    if (!instance) {
        instance = new constructorWindow();
    }
    return instance;
}

void constructorWindow::cleanup() {
    delete instance;
    instance = nullptr;
}

void constructorWindow::initScene() {
    mainScene = fScene.createSlotMachineScene();
    
    auto uiNode = fScene.fEntity.createNode();
    uiNode->setPos({-200.f,-100.f});
    uiNode->setAnchor({1.f, 1.f});
    fpsLabel = fScene.fEntity.createLabel();
    timeLabel = fScene.fEntity.createLabel();
    timeLabel->setPos({0.f,-20.f});
    mousePosLabel = fScene.fEntity.createLabel();
    mousePosLabel->setPos({0.f, 20.f});
    mainScene->addChild(uiNode);

    fpsLabel->getTransformComponent()->setPivot(tPivot::x, 1.f);
    timeLabel->getTransformComponent()->setPivot(tPivot::x, 1.f);
    mousePosLabel->getTransformComponent()->setPivot(tPivot::x, 1.f);

    uiNode->addChild(fpsLabel);
    uiNode->addChild(timeLabel);
    uiNode->addChild(mousePosLabel);
}

void constructorWindow::destroyScene() {
    fpsLabel.reset();
    timeLabel.reset();
    mousePosLabel.reset();
    mainScene.reset();
}
