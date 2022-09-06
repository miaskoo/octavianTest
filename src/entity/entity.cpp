#include "entity.h"
#include "transformComponent.h"
#include "labelComponent.h"
#include "bufferComponent.h"
#include "textureComponent.h"
#include <iostream>

entity::entity(dimension aType) : type(aType) {}

dimension entity::getDimension() {
    return type;
}

void entity::reserveChild(size_t count) {
    childs.reserve(count);
}

void entity::addChild(std::shared_ptr<entity>& child) {
    if (!child) {
        return;
    }
    auto d = getDimension();
    if (d != dimension::NONE) {
        if (child->getDimension() != getDimension()) {
            return;
        }
    }
    childs.push_back(child);
    child->setParent(wThis);
    markDirty();
}

void entity::removeChild(entity* child) {
    if (!child) {
        return;
    }
    
    auto iter = std::find_if(childs.begin(), childs.end(), [&child](auto& element){
        return child == element.get();
    });
    if (iter == childs.end()) {
        return;
    }
    
    child->setParent({});
    childs.erase(iter);
    markDirty();
}

void entity::setParent(std::weak_ptr<entity> aParent) {
    parent = aParent;
}

std::shared_ptr<entity> entity::getParent() {
    return parent.lock();
}

std::shared_ptr<entityCash> entity::getFreeCash() {
    auto cash = cashArray[0].use_count() == 1 ? cashArray[0] : cashArray[1];
    return cash;
}

std::shared_ptr<entityCash> entity::getBusyCash() {
    return cashArray[0].use_count() > 1 ? cashArray[0] : cashArray[1];
}

const std::vector<std::shared_ptr<entity>>& entity::getChilds() {
    return childs;
}

void entity::removeFromParent() {
    if (parent.expired()) {
        parent.lock()->removeChild(this);
        parent.lock()->markDirty();
    }
}

void entity::setWeakPointerThis(std::weak_ptr<entity> aWThis) {
    wThis = aWThis;
}

void entity::update(float dt) {
    for (auto& action : actions) {
        action->update(wThis, dt);
    }
    for (auto& child : childs) {
        child->update(dt);
    }
    auto forRemove = std::remove_if(actions.begin(), actions.end(), [this](auto& element){
        if (element->isEnd()) {
            element->end(wThis);
            return true;
        }
        return false;
    });
    if (forRemove != actions.end()) {
        actions.erase(forRemove);
    }
}

void entity::updateWithMousePos(int x, int y, stateMouse state) {
    for (auto& child : childs) {
        child->updateWithMousePos(x, y, state);
    }
}

void entity::addAction(actionBase *action) {
    actions.emplace_back(std::move(action));
}

void entity::clearAllActions() {
    actions.clear();
}

void entityCash::setOrtho(bool aOrtho) {
    ortho = aOrtho;
}

bool entityCash::isOrtho() {
    return ortho;
}

void entityCash::render() {
    for (const auto& component : components) {
        if (!component) {
            continue;
        }
        component->bind();
    }
    for (const auto& component : components) {
        if (!component) {
            continue;
        }
        component->use();
    }
    for (int n = 0; n < components.size(); n++) {
        auto& component = components[components.size() - 1 - n];
        if (!component) {
            continue;
        }
        component->unbind();
    }
}

bool entity::isDirty() {
    for (const auto& component : components) {
        if (component) {
            if (component->isDirty()) {
                return true;
            }
        }
    }
    return dirty;
}

void entity::markDirty() {
    dirty = true;
}

void entity::unDirty() {
    dirty = false;
    for (const auto& component : components) {
        if (component) {
            component->unDirty();
        }
    }
}

void entity::setIgnoreSorting(bool value) {
    ignoreSorting = value;
}

bool entity::isIgnoreSorting() {
    return ignoreSorting;
}
