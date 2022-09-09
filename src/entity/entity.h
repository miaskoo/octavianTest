#pragma once
#include <vector>
#include <array>
#include "struct.h"
#include "componentContainer.h"
#include "actionBase.h"

class transformComponentInterface;

enum class dimension {TWO = 2, THREE = 3, NONE = 0};

class entityCash : public componentContainer {
public:
    ~entityCash() = default;
    void render();
    bool isOrtho();
    void setOrtho(bool aOrtho);
protected:
    entityCash() = default;
    bool ortho = false;
};

class entity : public componentContainer {
    friend class factoryEntity;
    friend class constructorWindow;
public:
    
    ~entity() = default;
    dimension getDimension();
    
    std::shared_ptr<entity> getParent();
    const std::vector<std::shared_ptr<entity>>& getChilds();
    
    void reserveChild(size_t count);
    void addChild(std::shared_ptr<entity>& child);
    
    void removeChild(entity* child);
    void removeFromParent();
    
    void addAction(actionBase* action);
    void clearAllActions();
    
    virtual void updateWithMousePos(int x, int y, stateMouse state);
    
    bool isDirty();
    void markDirty();
    void unDirty();
    void setIgnoreSorting(bool value);
    bool isIgnoreSorting();
    
    virtual void updateCash() = 0;
    
    std::shared_ptr<entityCash> getFreeCash();
    std::shared_ptr<entityCash> getBusyCash();
    
    transformComponentInterface* getTransformComponent();
protected:
    entity() = delete;
    entity(dimension aType);
    
    void update(float dt);
    
    void setParent(std::weak_ptr<entity> aParent);
    void setWeakPointerThis(std::weak_ptr<entity> aWThis);
    
    virtual void createCash() {}
    
    std::array<std::shared_ptr<entityCash>, 2U> cashArray;
    std::weak_ptr<entity> wThis;
private:
    const dimension type;
    
    std::weak_ptr<entity> parent;
    std::vector<std::shared_ptr<entity>> childs;

    std::vector<std::unique_ptr<actionBase>> actions;
    
    bool dirty = false;
    bool ignoreSorting = false;
};
