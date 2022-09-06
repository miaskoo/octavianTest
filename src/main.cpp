
#include "constructorWindow.h"

int main(int argc, char **argv) {
    auto construct = constructorWindow::getInstance();
    construct->initWindow(argc, argv);
    construct->initResource();
    construct->initScene();
    construct->initGlutFunc();
    construct->startUpdateThread();
    construct->startMainLoop();
    std::atexit([](){
        constructorWindow::cleanup();
    });
    return 0;
}
