#ifndef MESH_PART_H
#define MESH_PART_H

#include "buffer/bufferViewImplementations.h"

class MeshPart
{
private:
    Vector3dBufferView<float>* _vertices;
    ScalarBufferView<short>* _indices;

public:
    MeshPart(Vector3dBufferView<float>* vertices, ScalarBufferView<short>* indices);
    void render();
};

#endif