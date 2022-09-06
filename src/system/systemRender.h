#pragma once
#include <list>
#include <atomic>
#include <memory>

class entityCash;

class systemRender {
public:
    systemRender() = default;
    ~systemRender() = default;
    void update();
    std::list<std::shared_ptr<entityCash>>& getCadrForCash();
    void markDirty();
    bool isDirty();
    void setWindowSize(int w, int h);
private:
    void setOrthographic();
    void setPerspective();
    void updateWindowSize();
    std::list<std::shared_ptr<entityCash>>& getCadr();
    std::list<std::shared_ptr<entityCash>> firstCadr;
    std::list<std::shared_ptr<entityCash>> secondCadr;
    bool nextCadr = false;
    std::atomic_bool dirty = false;
    bool ortho = false;
    
    int screenW = 0;
    int screenH = 0;
    bool windowSizeDirty = false;
};
