#pragma once
#include <vector>
#include <array>
#include "struct.h"
#include "componentContainer.h"
#include "actionBase.h"

class transformComponentInterface;

class entityCash : public componentContainer {
protected:
    ~entityCash() = default;
    entityCash() = default;
};

class entity : public componentContainer {
    friend class factoryEntity;
    friend class constructorWindow;
public:
    
    ~entity();
    dimension getDimension() const;
    
    std::shared_ptr<entity> getParent();
    const std::vector<std::shared_ptr<entity>>& getChilds();
    
    void reserveChild(size_t count);
    void addChild(std::shared_ptr<entity>& child);
    
    void removeChild(entity* child);
    void removeFromParent();
    
    void addAction(actionBase* action);
    void clearAllActions();
    
    bool isDirty() const;
    void markDirty();
    void unDirty();
    void setIgnoreSorting(bool value);
    bool isIgnoreSorting() const;
    
    virtual void updateCash(size_t freeCashIdx, size_t busyCashIdx) = 0;
    
    std::shared_ptr<entityCash> getCash(size_t cashIdx);
    
    transformComponentInterface* getTransformComponent();
protected:
    entity() = delete;
    entity(dimension aType);
    
    void update(float dt);
    
    void setParent(std::weak_ptr<entity> aParent);
    void setWeakPointerThis(std::weak_ptr<entity> aWThis);
    
    virtual void createCash() {}
    
    std::array<std::shared_ptr<entityCash>, static_cast<size_t>(typeCash::COUNT)> cashArray;
    std::weak_ptr<entity> wThis;
private:
    const dimension type;
    
    std::weak_ptr<entity> parent;
    std::vector<std::shared_ptr<entity>> childs;

    std::vector<std::unique_ptr<actionBase>> actions;
    
    bool dirty = false;
    bool ignoreSorting = false;
};
