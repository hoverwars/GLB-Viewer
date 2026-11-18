#ifndef NODE_H
#define NODE_H

#include <vector>
#include <string>
#include <glm/glm.hpp>

#include "mesh.h"

using namespace std;

class Node
{
private:
    Node* _father = nullptr;
    vector<Node*> _children;
    Mesh* _mesh;
    string _name;
    glm::vec4 _rotation;
    glm::vec3 _translation;
    glm::vec3 _scale;
    string _uniqueId;
    bool _isVisible;
    bool _selected = false;

public:
    Node(string name, vector<Node*> children, glm::vec4 rotation, glm::vec3 scale, glm::vec3 translation);
    Node(string name, Mesh* mesh, glm::vec4 rotation, glm::vec3 scale, glm::vec3 translation);

    glm::mat4 applyTransformations(glm::mat4 model);

    bool hasChild(Node* node);
    void setFather(Node* father);
    bool hasChild();
    vector<Node*> getChildren();
    void setName(string name);
    string getName();
    Mesh* getMesh();
    
    Node* getFather();
    glm::vec4 getRotation();      // Quaternion (x, y, z, w)
    glm::vec3 getTranslation();
    glm::vec3 getScale();
    void setRotation(glm::vec4 rotation);
    void setTranslation(glm::vec3 translation);
    void setScale(glm::vec3 scale);

    string getUniqueId();
    bool isVisible();
    bool isFatherVisible();  // If father == nullptr, returns true.
    void switchVisibility();
    void setSelected(bool selected);
    bool isSelected();
};

#endif