#include "node.h"

class button : public node {
public:
    button();
    ~button() = default;
    virtual void createCash() override;
    virtual void updateCash(size_t freeCashIdx, size_t busyCashIdx) override;
    virtual void updateWithMousePos(int x, int y, stateMouse state) override;
protected:
    class buttonCash : public nodeCash {
    public:
        buttonCash();
        ~buttonCash() = default;
    };
private:
    bool isClicked = false;
};
