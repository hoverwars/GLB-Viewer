#include "material.h"

#include <stdexcept>

using namespace std;

Material::Material(string name, bool doubleSided) : Material(name, doubleSided, { 1.0, 1.0, 1.0, 1.0 }, 1.0, 1.0)
{ }

Material::Material(string name, bool doubleSided, vector<float> pbrBaseColor, float pbrMetallicFactor, float pbrRoughnessFactor)
{
    if (pbrBaseColor.size() != 4) throw runtime_error("Cannot initialize PBR base color with an array of size different of 4");

    _textureBased = true;
    _name = name;
    _doubleSided = doubleSided;

    _pbrBaseColorFactor[0] = pbrBaseColor[0];
    _pbrBaseColorFactor[1] = pbrBaseColor[1];
    _pbrBaseColorFactor[2] = pbrBaseColor[2];
    _pbrBaseColorFactor[3] = pbrBaseColor[3];
    _pbrMetallicFactor = pbrMetallicFactor;
    _pbrRoughnessFactor = pbrRoughnessFactor;
}

float Material::getPbrRoughnessFactor()
{
    return _pbrRoughnessFactor;
}