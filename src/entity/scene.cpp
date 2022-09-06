#include "scene.h"

scene::scene() : entity(dimension::NONE) {}

void scene::updateDirtyCash(entity* obj) {
    for (auto& child : obj->getChilds()) {
        if (child->isDirty()) {
            child->updateCash();
        }
        updateDirtyCash(child.get());
    }
}

void scene::createNewCash(entity* obj, std::list<std::shared_ptr<entityCash>>& cashList) {
    updateDirtyCash(obj);
    for (const auto& dim : {dimension::THREE, dimension::TWO}) {
        for (auto& child : obj->getChilds()) {
            if (child->getDimension() != dim) {
                continue;
            }
            std::shared_ptr<entityCash> cash;
            if (child->isDirty()) {
                cash = child->getFreeCash();
            }
            else {
                cash = child->getBusyCash();
            }
            
            if (cash) {
                if (dim == dimension::THREE) {
                    cashList.push_front(cash);
                }
                else {
                    cash->setOrtho(true);
                    if (child->isIgnoreSorting()) {
                        cashList.push_front(cash);
                    }
                    else {
                        cashList.push_back(cash);
                    }
                    
                }
            }
            
            child->unDirty();
            createNewCash(child.get(), cashList);
        }
    }
}

void scene::checkDirty(entity* obj, bool& result) {
    if (result) {
        return;
    }
    for (auto& child : obj->getChilds()) {
        if (child->isDirty()) {
            result = true;
            break;
        }
        checkDirty(child.get(), result);
        if (result) {
            break;
        }
    }
}
