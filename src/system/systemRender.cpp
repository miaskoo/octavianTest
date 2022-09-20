#include "systemRender.h"
#include "entity.h"
#include "colorComponent.h"
#include "textureComponent.h"
#include "renderComponent.h"
#ifdef MACOS
#include "GLUT/glut.h"
#else
#include "freeglut/glut.h"
#endif
#include "glFunctional.h"

void systemRender::setOrthographic() {
    if (ortho) {
        return;
    }
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, screenW, screenH, 0);
    glMatrixMode(GL_MODELVIEW);
    ortho = true;
}

void systemRender::setPerspective() {
    if (!ortho) {
        return;
    }
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    ortho = false;
}

void systemRender::updateWindowSize() {
    setPerspective();
    
    float ratio =  screenW * 1.0 / screenH;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, screenW, screenH);
    gluPerspective(45.0f, ratio, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    gluLookAt(
        0.0, 0.0, 10.0,
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0);
}

void systemRender::update(size_t cashIdx) {
    if (windowSizeDirty) {
        updateWindowSize();
        windowSizeDirty = false;
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    for (auto object : objects) {
        if (object->getDimension() == dimension::TWO) {
           setOrthographic();
        }
        else {
           setPerspective();
        }
        renderEntity(object, cashIdx);
    }
    
    glutSwapBuffers();
}

void systemRender::renderEntity(entity* object, size_t cashIdx) {
    auto cash = object->getCash(cashIdx);
    
    bindColor(cash->getComponent<colorComponent>());
    bindTexture(cash->getComponent<textureComponent>());
    
    auto component = cash->getComponent<renderComponent>();
    switch (component->getRenderType()) {
        case typeRenderComponent::SPRITE: {
            renderSprite();
            break;
        }
        case typeRenderComponent::BUFFER: {
            auto castComponent = static_cast<bufferComponent*>(component);
            renderBuffer(castComponent->getBuffIdx(), castComponent->getBuffSize(), castComponent->isCullFaceMode());
            break;
        }
        case typeRenderComponent::LABEL: {
            auto castComponent = static_cast<labelComponent*>(component);
            renderLabel(castComponent->getText(), castComponent->getFont());
            break;
        }
        default: {
            break;
        }
    }
    unbindTexture();
    unbindColor();
}

void systemRender::bindColor(colorComponent *component) {
    if (!component) {
        return;
    }
    auto color = component->getColor();
    if (component->isAlphaMode()) {
        glForwarder::setAlphaMode(true);
        glForwarder::setColor4(color[0], color[1], color[2], color[3]);
    }
    else {
        glForwarder::setColor3(color[0], color[1], color[2]);
    }
}

void systemRender::unbindColor() {
    glForwarder::setColor4(255, 255, 255, 255);
}

void systemRender::bindTexture(textureComponent *component) {
    if (!component) {
        return;
    }
    glForwarder::bind2DTexture(component->getTexIdx());
    glForwarder::bindShaderProgram(component->getShaderIdx());
}

void systemRender::unbindTexture() {
    glForwarder::unbindShaderProgram();
    glForwarder::unbind2DTexture();
}

void systemRender::renderBuffer(size_t buffIdx, size_t buffSize, bool cullfaceMode) {
    glForwarder::bindVertexArray(buffIdx);
    glForwarder::setCullfaceMode(cullfaceMode);
    glForwarder::drawTriangleElements(buffSize);
    glForwarder::setCullfaceMode(false);
    glForwarder::unbindVertexArray();
}

void systemRender::renderLabel(const std::string &text, void *font) {
    glForwarder::drawBitmapText(text.c_str(), text.length(), font);
}

void systemRender::renderSprite() {
    glForwarder::drawTextureQuad(1);
}

void systemRender::setWindowSize(int w, int h) {
    screenW = w;
    screenH = h;
    windowSizeDirty = true;
}

void systemRender::registerEntity(entity* object) {
    if (auto component = object->getComponent<renderComponent>()) {
        objects.push_back(object);
    }
}

void systemRender::unregisterEntity(entity* object) {
    auto iter = std::remove_if(objects.begin(), objects.end(), [object](const auto& element){
        return object == element;
    });
    objects.erase(iter);
}

systemRender* systemRender::getInstance() {
    if (!instance) {
        instance = new systemRender();
    }
    return instance;
}

void systemRender::cleanup() {
    delete instance;
    instance = nullptr;
}
