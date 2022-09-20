#include "factoryEntity.h"
#include "node.h"
#include "label.h"
#include "object3d.h"
#include "sprite.h"
#include "button.h"
#include "transformComponent.h"
#include "renderComponent.h"

std::shared_ptr<entity> factoryEntity::createNode(vec2f pos, vec2f size) {
    auto result = std::make_shared<node>();
    result->setWeakPointerThis(result);
    auto component = result->getTransformComponent();
    component->setPos(pos);
    component->setSize(size);
    component->setAnchor({0.5f, 0.5f});
    component->setPivot({0.5f, 0.5f});
    component->setScale({1.f, 1.f});
    result->createCash();
    return result;
}

std::shared_ptr<entity> factoryEntity::createLabel(vec2f pos, const std::string& text, void* font) {
    auto result = std::make_shared<label>();
    result->setWeakPointerThis(result);
    auto component = result->getTransformComponent();
    component->setPos(pos);
    component->setAnchor({0.5f, 0.5f});
    component->setPivot({0.5f, 0.5f});
    component->setScale({1.f, 1.f});
    result->getComponent<labelComponent>()->setText(text);
    result->getComponent<labelComponent>()->setFont(font);
    result->createCash();
    return result;
}

std::shared_ptr<entity> factoryEntity::createQuadrilateral(vec3f pos, vec3f size, quaternion rotate) {
    auto result = std::make_shared<object3d>();
    result->setWeakPointerThis(result);
    auto component = result->getTransformComponent();
    component->setPos(pos);
    component->setSize(size);
    component->setRotate(rotate);
    component->setScale({1.f, 1.f, 1.f});
    component->setAnchor(vec3f({0.5f,0.5f,0.0f}));
    component->setPivot(vec3f({0.5f,0.5f,0.0f}));
    result->createCash();
    return result;
}

std::shared_ptr<entity> factoryEntity::createTorus(vec3f pos, vec3f size, int countSector, quaternion rotate) {
    auto result = std::make_shared<object3d>();
    result->setWeakPointerThis(result);
    auto component = result->getTransformComponent();
    component->setPos(pos);
    component->setSize(size);
    component->setRotate(rotate);
    component->setScale({1.f, 1.f, 1.f});
    component->setAnchor(vec3f({0.5f,0.5f,0.0f}));
    component->setPivot(vec3f({0.5f,0.5f,0.0f}));
    result->createCash();
    
    auto buff = fBuffer.getTorusBufferIdx(countSector);
    result->getComponent<bufferComponent>()->setBuffIdx(buff.vao);
    result->getComponent<bufferComponent>()->setBuffSize(buff.countIdx);
    result->getComponent<bufferComponent>()->setCullFaceMode(true);
    return result;
}

std::shared_ptr<entity> factoryEntity::createSprite(vec2f pos, vec2f size) {
    auto result = std::make_shared<sprite>();
    result->setWeakPointerThis(result);
    auto component = result->getTransformComponent();
    component->setPos(pos);
    component->setSize(size);
    component->setAnchor({0.5f,0.5f});
    component->setPivot({0.5f,0.5f});
    component->setScale({1.f,1.f});
    result->createCash();
    return result;
}

std::shared_ptr<entity> factoryEntity::createButton(vec2f pos, vec2f size) {
    std::shared_ptr<entity> result = std::make_shared<button>();
    result->setWeakPointerThis(result);
    auto component = result->getTransformComponent();
    component->setPos(pos);
    component->setSize(size);
    component->setScale({1.f, 1.f});
    component->setAnchor({0.5, 0.5});
    component->setPivot({0.5, 0.5});
    result->createCash();
    return result;
}
