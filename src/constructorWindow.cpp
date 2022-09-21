#include "constructorWindow.h"

#include "struct.h"
#include "scene.h"
#include "transformComponent.h"
#include "clickComponent.h"
#include "colorComponent.h"
#include "renderComponent.h"
#include "textureComponent.h"

#include "renderSystem.h"
#include "mouseSystem.h"

#include "factoryAction.h"

#include <stdio.h>
#include <iostream>
#include <math.h>
#include <vector>
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
    fEntity.getTextureFactory().loadTexturs();
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
    mainScene = std::make_shared<scene>();
    mainScene->setWeakPointerThis(mainScene);
    
    auto sceneSprite = fEntity.createSprite("scene.png");
    sceneSprite->setSize({static_cast<float>(screenW), static_cast<float>(screenH)});
    sceneSprite->setIgnoreSorting(true);
    mainScene->addChild(sceneSprite);
    
    auto s = fEntity.createSprite("slot.png");
    s->setPos({30, -40});
    s->setSize({1400, 850});
    mainScene->addChild(s);

    auto uiNode = fEntity.createNode();
    uiNode->setPos({-200.f,-100.f});
    uiNode->setAnchor({1.f, 1.f});
    fpsLabel = fEntity.createLabel();
    timeLabel = fEntity.createLabel();
    timeLabel->setPos({0.f,-20.f});
    mousePosLabel = fEntity.createLabel();
    mousePosLabel->setPos({0.f, 20.f});
    mainScene->addChild(uiNode);

    fpsLabel->getTransformComponent()->setPivot(tPivot::x, 1.f);
    timeLabel->getTransformComponent()->setPivot(tPivot::x, 1.f);
    mousePosLabel->getTransformComponent()->setPivot(tPivot::x, 1.f);

    uiNode->addChild(fpsLabel);
    uiNode->addChild(timeLabel);
    uiNode->addChild(mousePosLabel);
    
    auto buttonPlay = fEntity.createButton("wait.png", "cover.png", "press.png");
    buttonPlay->setSize({400, 100});
    buttonPlay->getTransformComponent()->setAnchor(tAnchor::y, 0.8f);
    mainScene->addChild(buttonPlay);
    
    auto labelButtonPlay = fEntity.createLabel("TOUCH ME");
    labelButtonPlay->setPos({0, 25});
    labelButtonPlay->setAnchor({0.5f,0.5f});
    labelButtonPlay->setPivot({0.5f,0.5f});
    labelButtonPlay->setScale({2.f, 2.f});
    buttonPlay->addChild(labelButtonPlay);
    
    std::weak_ptr<entity> labelWeak = labelButtonPlay;
    
    auto addActionChangeToLabel = [this, labelWeak](){
        color4b yellowColor = {MAX_COLOR,MAX_COLOR,0,MAX_COLOR};
        color4b blueColor = {0,0,MAX_COLOR,MAX_COLOR};
        color4b purpureColor = {MAX_COLOR,0,MAX_COLOR,MAX_COLOR};
        int timeChangeColor = 1000;
        
        if (auto labelButtonPlay = labelWeak.lock()) {
            labelButtonPlay->clearAllActions();
            auto color1 = factoryAction::createChangeColorAction(blueColor, timeChangeColor);
            auto color2 = factoryAction::createChangeColorAction(purpureColor, timeChangeColor);
            auto color3 = factoryAction::createChangeColorAction(yellowColor, timeChangeColor);
            auto actionSequence = factoryAction::createActionSequence({color1, color2, color3});
            auto actionRepeat = factoryAction::createRepeatInfinityAction(actionSequence);
            labelButtonPlay->addAction(actionRepeat);
        }
    };
    addActionChangeToLabel();
    
    const size_t countTorus = 5U;
    std::function<void(std::weak_ptr<entity>, float)> rotateTorusAction[countTorus];
    std::weak_ptr<entity> wTorusPull[countTorus];
    
    for (size_t n = 0; n < countTorus; n++) {
        auto w = 3.f;
        auto h = 3.f;
        auto z = 1.2f;
        auto smeh = 0.0f;
        
        //поскольку центр у объекта смещен в 0.5 а не 0.0 требуется добавить половину size для правильной работы anchor и pivot
        auto size = vec3f({w,h,z});
        auto customX = z + smeh;
        float x = -(customX * 2) + customX * n;
        auto pos = vec3f({x,0,0});
        pos[0] += size[0] * 0.5f;
        pos[1] += size[1] * 0.5f;
        
        auto torus = fEntity.createTorus(8);
        torus->setPos(pos);
        torus->setSize(size);
        torus->setRotate({90, 0, 90});
        auto component = torus->getComponent<textureComponent>();
        component->setTexIdx(fEntity.getTextureFactory().getTextureIdx("slotTorus.png"));
        component->setShaderIdx(fEntity.getTextureFactory().getShaderTextureIdx());
        auto tComponentTorus = torus->getTransformComponent();
        mainScene->addChild(torus);
        
        wTorusPull[n] = torus;
        
        rotateTorusAction[n] = [this](std::weak_ptr<entity> torusWeak, float speed){
            if (auto torus = torusWeak.lock()) {
                auto actionRot = factoryAction::createRotateAction(quaternion::axisX, -360.f, speed);
                auto actionRep = factoryAction::createRepeatInfinityAction(actionRot);
                torus->addAction(actionRep);
            }
        };
    }
    std::weak_ptr<entity> wButton = buttonPlay;
    buttonPlay->getComponent<clickComponent>()->setClickCallback([this, wButton, wTorusPull, rotateTorusAction, labelWeak, addActionChangeToLabel]() {
        for (int n = 0; n < 5; n++) {
            if (rotateTorusAction[n]) {
                float randSpeed = 1100 - (100 * (rand() % 8));
                rotateTorusAction[n](wTorusPull[n], randSpeed);
            }
            int waitTime = 1000 + 500 * n;
            mainScene->addAction(factoryAction::createDelayAction(waitTime, [this, wTorus = wTorusPull[n]](){
                if (auto torus = wTorus.lock()) {
                    torus->clearAllActions();
                    auto q = torus->getTransformComponent()->getRotate();
                    auto euler = quaternion::convertToEuler3f(q);
                    auto step =  360.f / 8.f;
                    auto angle = euler.y();
                    size_t countStep = 0U;
                    while (angle > step) {
                        countStep++;
                        angle -= step;
                    }
                    angle = -22.5f - step * (countStep + 1);
                    auto result = quaternion::getFromEuler3(90, angle, 90);
                    torus->addAction(factoryAction::createRotateLerpAction(result, 500));
                }
            }));
        }
        mainScene->addAction(factoryAction::createDelayAction(3100, [wButton, labelWeak, addActionChangeToLabel](){
            if (auto buttonPlay = wButton.lock()) {
                buttonPlay->getComponent<clickComponent>()->setClickable(true);
            }
            addActionChangeToLabel();
        }));
        if (auto buttonPlay = wButton.lock()) {
            buttonPlay->getComponent<clickComponent>()->setClickable(false);
        }
        if (auto labelButtonPlay = labelWeak.lock()) {
            labelButtonPlay->clearAllActions();
            labelButtonPlay->getComponent<colorComponent>()->setColor(0, 0, 0, MAX_COLOR);
        }
    });
}

void constructorWindow::destroyScene() {
    fpsLabel.reset();
    timeLabel.reset();
    mousePosLabel.reset();
    mainScene.reset();
}
