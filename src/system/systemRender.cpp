#include "systemRender.h"
#include "entity.h"
#include "freeglut/glut.h"

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

std::list<std::shared_ptr<entityCash>>& systemRender::getCadr() {
    return nextCadr ? firstCadr : secondCadr;
}

std::list<std::shared_ptr<entityCash>>& systemRender::getCadrForCash() {
    return nextCadr ? secondCadr : firstCadr;
}

void systemRender::markDirty() {
    dirty.store(true);
}

bool systemRender::isDirty() {
    return dirty.load();
}

void systemRender::update() {
    if (isDirty()) {
        nextCadr = !nextCadr;
        dirty.store(false);
    }
    if (windowSizeDirty) {
        updateWindowSize();
        windowSizeDirty = false;
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    auto& objects = getCadr();
    
    for (auto object : objects) {
        if (object->isOrtho()) {
           setOrthographic();
        }
        else {
           setPerspective();
        }
        object->render();
    }
    
    glutSwapBuffers();
}

void systemRender::setWindowSize(int w, int h) {
    screenW = w;
    screenH = h;
    windowSizeDirty = true;
}
