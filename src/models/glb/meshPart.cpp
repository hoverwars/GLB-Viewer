#include "meshPart.h"

MeshPart::MeshPart(Vector3dBufferView<float>* vertices, ScalarBufferView<unsigned short>* indices)
{
    _vertices = vertices;
    _indices = indices;
}

Vector3dBufferView<float>* MeshPart::getVertices()
{
    return _vertices;
}

ScalarBufferView<unsigned short>* MeshPart::getIndices()
{
    return _indices;
}
