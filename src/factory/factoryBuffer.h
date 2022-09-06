#pragma once

#include <unordered_map>
#include <map>
#include <string>
#include <vector>
#include <GLUT/glut.h>

struct bufferIdx {
    GLuint vbo;
    GLuint vao;
    GLuint ebo;
    GLuint countIdx;
};

class factoryBuffer {
public:
    factoryBuffer() = default;
    ~factoryBuffer() = default;
    bufferIdx getTorusBufferIdx(int countSector);
private:
    void createTorus(int countSector);
    std::unordered_map<size_t, bufferIdx> torusSectorToBuf;
};
