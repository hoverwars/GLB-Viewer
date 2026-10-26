#include "meshPart.h"

#include "../../utils/logs.h"
#include <format>
using namespace std;

MeshPart::MeshPart(Vector3dBufferView<float>* vertices, ScalarBufferView<unsigned short>* indices)
{
    _vertices = vertices;
    _indices = indices;
    logInfo(format("{} vertexs, {} index chargés", vertices->getData().size(), indices->getData().size()));
}

Vector3dBufferView<float>* MeshPart::getVertices()
{
    return _vertices;
}

ScalarBufferView<unsigned short>* MeshPart::getIndices()
{
    return _indices;
}
