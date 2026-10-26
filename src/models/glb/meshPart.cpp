#include "meshPart.h"

MeshPart::MeshPart(Vector3dBufferView<float>* vertices, ScalarBufferView<short>* indices)
{
    _vertices = vertices;
    _indices = indices;
}

void MeshPart::render()
{
    
}