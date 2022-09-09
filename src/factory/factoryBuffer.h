#pragma once

#include <unordered_map>
#include <map>
#include <string>
#include <vector>

struct bufferIdx {
    unsigned int vbo;
    unsigned int vao;
    unsigned int ebo;
    unsigned int countIdx;
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
