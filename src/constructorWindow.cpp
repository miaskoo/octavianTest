#include "constructorWindow.h"

#include "struct.h"
#include "scene.h"
#include "transformComponent.h"
#include "clickComponent.h"
#include "labelComponent.h"
#include "colorComponent.h"

#include <stdio.h>
#include <iostream>
#include <math.h>
#include <vector>
#include <chrono>
#include <ctime>
#include <thread>

void constructorWindow::initWindow(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(0,0);
    glutInitWindowSize(screenW,screenH);
    sRender.setWindowSize(screenW, screenH);
    srand(time(0U));
    glutCreateWindow("test");
    glewInit();
}

void constructorWindow::initResource() {
    fTexture.loadTexturs();
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
    mainScene->updateWithMousePos(mouseX.load(), mouseY.load(), clickState);
    if (clickState == stateMouse::CLICK_OUT) {
        click.store(stateMouse::IDLE);
    }
    mainScene->update(dt);
    if (sRender.isDirty()) {
        return;
    }
    bool dirty = false;
    mainScene->checkDirty(mainScene.get(), dirty);
    if (dirty) {
        auto& cadrForCash = sRender.getCadrForCash();
        cadrForCash.clear();
        mainScene->createNewCash(mainScene.get(), cadrForCash);
        sRender.markDirty();
    }
}

void constructorWindow::renderScene() {
    if (currentFps < lockFps) {
        sRender.update();
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
        instance = new constructorWindow;
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
    
    auto sceneSprite = fEntity.createSprite({0.f,0.f}, {static_cast<float>(screenW), static_cast<float>(screenH)});
    sceneSprite->setIgnoreSorting(true);
    sceneSprite->getComponent<textureComponent>()->setTexIdx(fTexture.getTextureIdx("scene.png"));
    mainScene->addChild(sceneSprite);
    
    auto s = fEntity.createSprite(vec2f(30, -40), vec2f(1400, 850));
    s->getComponent<textureComponent>()->setTexIdx(fTexture.getTextureIdx("slot.png"));
    mainScene->addChild(s);

    auto uiNode = fEntity.createNode({-200.f,-100.f});
    uiNode->getComponent<transformComponent>()->setAnchor({1.f, 1.f});
    fpsLabel = fEntity.createLabel();
    timeLabel = fEntity.createLabel({0.f,-20.f});
    mousePosLabel = fEntity.createLabel({0.f, 20.f});
    mainScene->addChild(uiNode);

    fpsLabel->getComponent<transformComponent>()->setPivot(tPivot::x, 1.f);
    timeLabel->getComponent<transformComponent>()->setPivot(tPivot::x, 1.f);
    mousePosLabel->getComponent<transformComponent>()->setPivot(tPivot::x, 1.f);

    uiNode->addChild(fpsLabel);
    uiNode->addChild(timeLabel);
    uiNode->addChild(mousePosLabel);
    
    auto buttonPlay = fEntity.createButton({0,0}, {400, 100});
    buttonPlay->getComponent<buttonComponent>()->setTexIdx(
        fTexture.getTextureIdx("wait.png"),
        fTexture.getTextureIdx("hover.png"),
        fTexture.getTextureIdx("press.png"));
    buttonPlay->getComponent<transformComponent>()->setAnchor(tAnchor::y, 0.8f);
    mainScene->addChild(buttonPlay);
    
    auto labelButtonPlay = fEntity.createLabel({0, 25}, "TOUCH ME");
    labelButtonPlay->getComponent<transformComponent>()->setAnchor({0.5f,0.5f});
    labelButtonPlay->getComponent<transformComponent>()->setPivot({0.5f,0.5f});
    labelButtonPlay->getComponent<transformComponent>()->setScale({2.f, 2.f});
    buttonPlay->addChild(labelButtonPlay);
    
    std::weak_ptr<entity> labelWeak = labelButtonPlay;
    
    auto addActionChangeToLabel = [this, labelWeak](){
        vec4f yellowColor = {1,1,0,1};
        vec4f blueColor = {0,0,1,1};
        vec4f purpureColor = {1,0,1,1};
        int timeChangeColor = 1000;
        
        if (auto labelButtonPlay = labelWeak.lock()) {
            labelButtonPlay->clearAllActions();
            auto color1 = fAction.createChangeColorAction(yellowColor, blueColor, timeChangeColor);
            auto color2 = fAction.createChangeColorAction(blueColor, purpureColor, timeChangeColor);
            auto color3 = fAction.createChangeColorAction(purpureColor, yellowColor, timeChangeColor);
            auto actionSequence = fAction.createActionSequence({color1, color2, color3});
            auto actionRepeat = fAction.createRepeatInfinityAction(actionSequence);
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
        
        //поскольку центр у объекта смещен в 0.0 а не 0.5 требуется добавить половину size для правильной работы anchor и pivot
        auto size = vec3f({w,h,z});
        auto customX = z + smeh;
        float x = -(customX * 2) + customX * n;
        auto pos = vec3f({x,0,0});
        pos[0] += size[0] * 0.5f;
        pos[1] += size[1] * 0.5f;
        
        auto torus = fEntity.createTorus(pos, size, 8, quaternion::getFromEuler3(0, 90, 0));
        auto component = torus->getComponent<textureComponent>();
        component->setTexIdx(fTexture.getTextureIdx("slotTorus.png"));
        component->setShaderIdx(fTexture.getShaderTextureIdx());
        auto tComponentTorus = torus->getComponent<transformComponent>();
        tComponentTorus->setRotate(tComponentTorus->getRotate() * quaternion::getFromEuler(quaternion::axisX, 22.5f));
        mainScene->addChild(torus);
        
        wTorusPull[n] = torus;
        
        rotateTorusAction[n] = [this](std::weak_ptr<entity> torusWeak, float speed){
            if (auto torus = torusWeak.lock()) {
                quaternion startRotate = torus->getComponent<transformComponent>()->getRotate();
                auto actionRot = fAction.createRotateAction(quaternion::axisX, -360.f, speed, startRotate);
                auto actionRep = fAction.createRepeatInfinityAction(actionRot);
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
            mainScene->addAction(fAction.createDelayAction(waitTime, [this, wTorus = wTorusPull[n]](){
                if (auto torus = wTorus.lock()) {
                    torus->clearAllActions();
                    auto needRotate = quaternion::getFromEuler3(0, 90, 0);
                    auto angle = (rand() % 6) * (360 / 8);
                    needRotate = needRotate * quaternion::getFromEuler(quaternion::axisX, 22.5f + angle);
                    torus->getComponent<transformComponent>()->setRotate(needRotate);
                }
            }));
        }
        mainScene->addAction(fAction.createDelayAction(3100, [wButton, labelWeak, addActionChangeToLabel](){
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
            labelButtonPlay->getComponent<colorComponent>()->setColor(0, 0, 0, 1);
        }
    });
}

void constructorWindow::destroyScene() {
    fpsLabel.reset();
    timeLabel.reset();
    mousePosLabel.reset();
    mainScene.reset();
}
