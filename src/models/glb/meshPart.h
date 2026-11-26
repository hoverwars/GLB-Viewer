#ifndef MESH_PART_H
#define MESH_PART_H

#include "buffer/bufferViewImplementations.h"

class MeshPart
{
private:
    Vector3dBufferView<float>* _vertices;
    ScalarBufferView<unsigned short>* _indices;
    Vector2dBufferView<float>* _texture;

public:
    MeshPart(Vector3dBufferView<float>* vertices, ScalarBufferView<unsigned short>* indices, Vector2dBufferView<float>* texture);
    Vector3dBufferView<float>* getVertices();
    ScalarBufferView<unsigned short>* getIndices();
    Vector2dBufferView<float>* getTexture();
};

#endif