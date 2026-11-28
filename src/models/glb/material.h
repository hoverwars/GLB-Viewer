#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include <glm/glm.hpp>

using namespace std;

class Material
{
private:
    string _name;
    bool _doubleSided;
    bool _textureBased;

    // Base PBR data, from the GLTF file
    glm::vec4 _pbrBaseColor;
    float _pbrMetallicFactor;
    float _pbrRoughnessFactor;


public:
    Material(string name, bool doubleSided);
    Material(string name, bool doubleSided, glm::vec4 pbrBaseColorFactor, float pbrMetallicFactor, float pbrRoughnessFactor);

    float getPbrRoughnessFactor();
    glm::vec4 getPbrBaseColor();
};

#endif