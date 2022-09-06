#pragma once
#include <stdio.h>
#include "struct.h"
#include "entity.h"
#include "componentBase.h"
#include "constructorWindow.h"
#include "GLUT/glut.h"

enum class tPos{x = 0U, y = 1U, z = 2U};
enum class tSize{width = 0U, height = 1U, depth = 2U};
enum class tAnchor{x = 0U, y = 1U, z = 2U};
enum class tPivot{x = 0U, y = 1U, z = 2U};
enum class tScale{x = 0U, y = 1U, z = 2U};

class transformComponentInterface {
public:
    virtual vec3f getPos() const = 0;
    virtual vec3f getAnchor() const = 0;
    virtual vec3f getPivot() const = 0;
    virtual vec3f getScale() const = 0;
    virtual vec3f getSize() const = 0;
    
    virtual float getPos(tPos typePos) const = 0;
    virtual float getAnchor(tAnchor typeAnchor) const = 0;
    virtual float getPivot(tPivot typePivot) const = 0;
    virtual float getScale(tScale typeScale) const = 0;
    virtual float getSize(tSize typeSize) const = 0;
    
    virtual void setPos(const vec3f& aPos) = 0;
    virtual void setAnchor(const vec3f& aPos) = 0;
    virtual void setPivot(const vec3f& aPos) = 0;
    virtual void setScale(const vec3f& aPos) = 0;
    virtual void setSize(const vec3f& aPos) = 0;
    
    virtual void setPos(tPos type, float aValue) = 0;
    virtual void setAnchor(tAnchor type, float aValue) = 0;
    virtual void setPivot(tPivot type, float aValue) = 0;
    virtual void setScale(tScale type, float aValue) = 0;
    virtual void setSize(tSize type, float aValue) = 0;
    
    virtual quaternion getRotate() = 0;
    virtual void setRotate(int x, int y, int z) = 0;
    virtual void setRotate(const quaternion &aRotate) = 0;
    
    virtual vec3f getRealPos(size_t countDimension, std::weak_ptr<entity> parent) = 0;
    
    virtual void updateCashTransform(std::weak_ptr<entity> wThis) = 0;
protected:
    transformComponentInterface() = default;
    virtual ~transformComponentInterface() = default;
};

class transformComponent : public componentBase<componentId::TRANSFORM>, public transformComponentInterface {
public:
    transformComponent();
    ~transformComponent() = default;
    
    vec3f getCashPos() const;
    vec3f getCashSize() const;
    GLfloat const* getCashRotate() const;
    
    void setCashPos(vec3f aPos);
    void setCashSize(vec3f aSize);
    void setCashRotate(float* aRotate);

    void bind() const override;
    void unbind() const override;
    void use() const override {}
private:
    vec3f cashPos;
    vec3f cashSize;
    GLfloat cashRotate[16];
};

class transformComponentEmptyInterface : public transformComponent {
public:
    transformComponentEmptyInterface() = default;
    ~transformComponentEmptyInterface() = default;

    virtual vec3f getPos() const override {return {};}
    virtual vec3f getAnchor() const override {return {};}
    virtual vec3f getPivot() const override {return {};}
    virtual vec3f getScale() const override {return {};}
    virtual vec3f getSize() const override {return {};}

    virtual float getPos(tPos typePos) const override {return {};}
    virtual float getAnchor(tAnchor typeAnchor) const override {return {};}
    virtual float getPivot(tPivot typePivot) const override {return {};}
    virtual float getScale(tScale typeScale) const override {return {};}
    virtual float getSize(tSize typeSize) const override {return {};}
    
    virtual void setPos(const vec3f& aPos) override {}
    virtual void setAnchor(const vec3f& aPos) override {}
    virtual void setPivot(const vec3f& aPos) override {}
    virtual void setScale(const vec3f& aPos) override {}
    virtual void setSize(const vec3f& aPos) override {}
    
    virtual void setPos(tPos type, float aValue) override {}
    virtual void setAnchor(tAnchor type, float aValue) override {}
    virtual void setPivot(tPivot type, float aValue) override {}
    virtual void setScale(tScale type, float aValue) override {}
    virtual void setSize(tSize type, float aValue) override {}
    
    virtual quaternion getRotate() override {return {};}
    virtual void setRotate(int x, int y, int z) override {}
    virtual void setRotate(const quaternion &aRotate) override {}
    
    virtual vec3f getRealPos(size_t countDimension, std::weak_ptr<entity> parent) override {return {};}
    
    virtual void updateCashTransform(std::weak_ptr<entity> wThis) override {};
};

template <typename T>
class transformComponentMain : public transformComponent {
public:
    transformComponentMain() = default;
    ~transformComponentMain() = default;
    
    virtual vec3f getPos() const override {
        vec3f result;
        for (size_t n = 0U; n < pos.size(); n++) {
            result[n] = pos[n];
        }
        return result;
    }
    virtual vec3f getSize() const override {
        vec3f result;
        for (size_t n = 0U; n < size.size(); n++) {
            result[n] = size[n];
        }
        return result;
    }
    virtual vec3f getAnchor() const override {
        vec3f result;
        for (size_t n = 0U; n < anchor.size(); n++) {
            result[n] = anchor[n];
        }
        return result;
    }
    virtual vec3f getPivot() const override {
        vec3f result;
        for (size_t n = 0U; n < pivot.size(); n++) {
            result[n] = pivot[n];
        }
        return result;
    }
    virtual vec3f getScale() const override {
        vec3f result;
        for (size_t n = 0U; n < scale.size(); n++) {
            result[n] = scale[n];
        }
        return result;
    }
    
    virtual float getPos(tPos typePos) const override {
        return pos[static_cast<size_t>(typePos)];
    }
    virtual float getSize(tSize typeSize) const override {
        return size[static_cast<size_t>(typeSize)];
    }
    virtual float getAnchor(tAnchor typeAnchor) const override {
        return anchor[static_cast<size_t>(typeAnchor)];
    }
    virtual float getPivot(tPivot typePivot) const override {
        return pivot[static_cast<size_t>(typePivot)];
    }
    virtual float getScale(tScale typeScale) const override {
        return scale[static_cast<size_t>(typeScale)];
    }

    virtual void setPos(const vec3f& aValue) override {
        for (size_t n = 0U; n < pos.size(); n++) {
            pos[n] = aValue[n];
        }
        markDirty();
    }
    virtual void setSize(const vec3f& aValue) override {
        for (size_t n = 0U; n < size.size(); n++) {
            size[n] = aValue[n];
        }
        markDirty();
    }
    virtual  void setAnchor(const vec3f& aValue) override {
        for (size_t n = 0U; n < size.size(); n++) {
            anchor[n] = aValue[n];
        }
        markDirty();
    }
    virtual void setPivot(const vec3f& aValue) override {
        for (size_t n = 0U; n < size.size(); n++) {
            pivot[n] = aValue[n];
        }
        markDirty();
    }
    virtual void setScale(const vec3f& aValue) override {
        for (size_t n = 0U; n < size.size(); n++) {
            scale[n] = aValue[n];
        }
        markDirty();
    }
    
    virtual void setPos(tPos type, float aValue) override {
        pos[static_cast<size_t>(type)] = aValue;
        markDirty();
    }
    virtual void setSize(tSize type, float aValue) override {
        size[static_cast<size_t>(type)] = aValue;
        markDirty();
    }
    virtual void setAnchor(tAnchor type, float aValue) override {
        anchor[static_cast<size_t>(type)] = aValue;
        markDirty();
    }
    virtual void setPivot(tPivot type, float aValue) override {
        pivot[static_cast<size_t>(type)] = aValue;
        markDirty();
    }
    virtual void setScale(tScale type, float aValue) override {
        scale[static_cast<size_t>(type)] = aValue;
        markDirty();
    }
    
    virtual quaternion getRotate() override {
        return rotate;
    }
    virtual void setRotate(int x, int y, int z) override {
        rotate = quaternion::getFromEuler3(x, y, z);
        markDirty();
    }
    virtual void setRotate(const quaternion &aRotate) override {
        rotate = aRotate;
        markDirty();
    }
    
    virtual vec3f getRealPos(size_t countDimension, std::weak_ptr<entity> parent) override {
        vec3f result;
        vec3f posParent;
        vec3f sizeParent;
        auto pParent = parent.lock();
        transformComponent* parentTransform = nullptr;
        if (pParent) {
            parentTransform = pParent->getComponent<transformComponent>();
        }
        if (pParent && parentTransform) {
            posParent = parentTransform->getCashPos();
            sizeParent = parentTransform->getCashSize();
        }
        else if (countDimension == 2U) {
            sizeParent[0] = constructorWindow::getInstance()->getScreenW();
            sizeParent[1] = constructorWindow::getInstance()->getScreenH();
        }
        else if (countDimension == 3U) {
            sizeParent[0] = 0;
            sizeParent[1] = 0;
            sizeParent[2] = 0;
        }
        for (int n = 0; n < static_cast<int>(2); n++) {
            result[n] = posParent[n] + pos[n];
            result[n] += sizeParent[n] * anchor[n];
            result[n] += (-size[n] + size[n] * pivot[n]) * scale[n];
        }
        return result;
    }
    
    virtual void updateCashTransform(std::weak_ptr<entity> wThis) override {
        if (isDirty()) {
            if (auto sThis = wThis.lock()) {
                setCashPos(getRealPos(static_cast<size_t>(sThis->getDimension()), sThis->getParent()));
                setCashSize(getSize() * getScale());
                setCashRotate(quaternion::convertToMat3f(getRotate()).get());
            }
        }
    }
private:
    T pos;
    T size;
    T anchor;
    T pivot;
    T scale;
    quaternion rotate;
};
