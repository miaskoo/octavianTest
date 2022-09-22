#include "factoryScene.h"

#include "struct.h"
#include "scene.h"

#include "transformComponent.h"
#include "clickComponent.h"
#include "colorComponent.h"
#include "renderComponent.h"
#include "textureComponent.h"

#include "factoryAction.h"
#include "factoryEntity.h"
#include "textureController.h"

#include <iostream>
#include <math.h>
#include <vector>

std::shared_ptr<scene> factoryScene::createGameScene() {
    auto mainScene = std::make_shared<scene>();
    mainScene->setWeakPointerThis(mainScene);
    
    auto sceneSprite = factoryEntity::createSprite("scene.png");
    sceneSprite->setSize({static_cast<float>(constructorWindow::getInstance()->getScreenW()), static_cast<float>(constructorWindow::getInstance()->getScreenH())});
    sceneSprite->setIgnoreSorting(true);
    mainScene->addChild(sceneSprite);
    
    auto s = factoryEntity::createSprite("slot.png");
    s->setPos(30, -40);
    s->setSize(1400, 850);
    mainScene->addChild(s);
    
    auto buttonPlay = factoryEntity::createButton("wait.png", "cover.png", "press.png");
    buttonPlay->setSize(400, 100);
    buttonPlay->getTransformComponent()->setAnchor(tAnchor::y, 0.8f);
    mainScene->addChild(buttonPlay);
    
    auto labelButtonPlay = factoryEntity::createLabel("TOUCH ME");
    labelButtonPlay->setPos(0, 25);
    labelButtonPlay->setAnchor(0.5f,0.5f);
    labelButtonPlay->setPivot(0.5f,0.5f);
    labelButtonPlay->setScale(2.f, 2.f);
    buttonPlay->addChild(labelButtonPlay);
    
    std::weak_ptr<entity> labelWeak = labelButtonPlay;
    
    auto addActionChangeToLabel = [labelWeak](){
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
        
        auto torus = factoryEntity::createTorus(8);
        torus->setPos(pos);
        torus->setSize(size);
        torus->setRotate(90, 0, 90);
        auto component = torus->getComponent<textureComponent>();
        auto texController = constructorWindow::getInstance()->getTextureController();
        component->setTexIdx(texController->getTextureIdx("slotTorus.png"));
        component->setShaderIdx(texController->getDefaultShaderProgramIdx());
        auto tComponentTorus = torus->getTransformComponent();
        mainScene->addChild(torus);
        
        wTorusPull[n] = torus;
        
        rotateTorusAction[n] = [](std::weak_ptr<entity> torusWeak, float speed){
            if (auto torus = torusWeak.lock()) {
                auto actionRot = factoryAction::createRotateAction(quaternion::axisX, -180.f, speed);
                auto actionRep = factoryAction::createRepeatInfinityAction(actionRot);
                torus->addAction(actionRep);
            }
        };
    }
    std::weak_ptr<entity> wButton = buttonPlay;
    buttonPlay->getComponent<clickComponent>()->setClickCallback([wButton, wTorusPull, rotateTorusAction, labelWeak, addActionChangeToLabel, mainScene]() {
        for (int n = 0; n < 5; n++) {
            if (rotateTorusAction[n]) {
                float randSpeed = 500 - (50 * (rand() % 5));
                rotateTorusAction[n](wTorusPull[n], randSpeed);
            }
            int waitTime = 1000 + 500 * n;
            mainScene->addAction(factoryAction::createDelayAction(waitTime, [wTorus = wTorusPull[n]](){
                if (auto torus = wTorus.lock()) {
                    torus->clearAllActions();
                    auto q = torus->getTransformComponent()->getRotate();
                    auto rotate = q.convertToEuler3f();
                    auto step =  360.f / 8.f;
                    auto angle = rotate.x();
                    while (angle > step) {
                        angle -= step;
                    }
                    if (angle < 0.f) {
                        angle *= -1;
                        angle = step - angle;
                    }
                    angle += step + 22.5;
                    q = q * quaternion::getFromEuler(quaternion::axisX, -angle);
                    torus->addAction(factoryAction::createRotateLerpAction(q, 500));
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
    
    return mainScene;
}
