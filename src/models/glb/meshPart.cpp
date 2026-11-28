#include "meshPart.h"

MeshPart::MeshPart(Vector3dBufferView<float>* vertices, ScalarBufferView<unsigned short>* indices, Vector2dBufferView<float>* texture, Material* material)
{
    _vertices = vertices;
    _indices = indices;
    _texture = texture;
    _material = material;
}

Vector3dBufferView<float>* MeshPart::getVertices()
{
    return _vertices;
}

ScalarBufferView<unsigned short>* MeshPart::getIndices()
{
    return _indices;
}

Vector2dBufferView<float>* MeshPart::getTexture()
{
    return _texture;
}

Material* MeshPart::getMaterial()
{
    return _material;
}