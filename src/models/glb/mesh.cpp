#include "mesh.h"

Mesh::Mesh(string name, vector<MeshPart*> parts)
{
    _name = name;
    _parts = parts;
}

Mesh::~Mesh()
{
    for (MeshPart* part : _parts)
    {
        delete part;
    }
}

vector<MeshPart*> Mesh::getParts()
{
    return _parts;
}