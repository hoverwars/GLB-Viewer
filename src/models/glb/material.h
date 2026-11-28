#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include <vector>

using namespace std;

class Material
{
private:
    string _name;
    bool _doubleSided;
    bool _textureBased;

    // Base PBR data, from the GLTF file
    float _pbrBaseColorFactor[4];
    float _pbrMetallicFactor;
    float _pbrRoughnessFactor;


public:
    Material(string name, bool doubleSided);
    Material(string name, bool doubleSided, vector<float> pbrBaseColorFactor, float pbrMetallicFactor, float pbrRoughnessFactor);

    float getPbrRoughnessFactor();
};

#endif