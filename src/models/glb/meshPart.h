#ifndef MESH_PART_H
#define MESH_PART_H

#include "buffer/bufferViewImplementations.h"
#include "material.h"

class MeshPart
{
private:
    Vector3dBufferView<float>* _vertices;
    ScalarBufferView<unsigned short>* _indices;
    Vector2dBufferView<float>* _texture;
    Material* _material;

public:
    MeshPart(Vector3dBufferView<float>* vertices, ScalarBufferView<unsigned short>* indices, Vector2dBufferView<float>* texture, Material* material);
    Vector3dBufferView<float>* getVertices();
    ScalarBufferView<unsigned short>* getIndices();
    Vector2dBufferView<float>* getTexture();
    Material* getMaterial();
};

#endif