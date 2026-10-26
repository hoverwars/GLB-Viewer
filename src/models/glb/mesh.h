#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>

#include "meshPart.h"

using namespace std;

class Mesh
{
private:
    string _name;
    vector<MeshPart*> _parts;

public:
    Mesh(string name, vector<MeshPart*> parts);
    ~Mesh();
    vector<MeshPart*> getParts();
};

#endif