#include "material.h"

#include <stdexcept>

#include "../../utils/darkStyle.h"
#include "../../utils/convert.h"

using namespace std;

Material::Material(string name, bool doubleSided) : Material(name, doubleSided, toGlmVector(shaderFillColor), 1.0, 1.0)
{ }

Material::Material(string name, bool doubleSided, glm::vec4 pbrBaseColor, float pbrMetallicFactor, float pbrRoughnessFactor)
{
    _textureBased = true;
    _name = name;
    _doubleSided = doubleSided;

    _pbrBaseColor = pbrBaseColor;
    _pbrMetallicFactor = pbrMetallicFactor;
    _pbrRoughnessFactor = pbrRoughnessFactor;
}

float Material::getPbrRoughnessFactor()
{
    return _pbrRoughnessFactor;
}

glm::vec4 Material::getPbrBaseColor()
{
    return _pbrBaseColor;
}