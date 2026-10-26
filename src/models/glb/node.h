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
    vector<Node*> _children;
    Mesh* _mesh;
    string _name;
    glm::vec4 _rotation;
    glm::vec3 _translation;
    glm::vec3 _scale;

public:
    Node(string name, vector<Node*> children, glm::vec4 rotation, glm::vec3 scale, glm::vec3 translation);
    Node(string name, Mesh* mesh, glm::vec4 rotation, glm::vec3 scale, glm::vec3 translation);

    bool hasChild(Node* node);
    bool hasChild();
    vector<Node*> getChildren();
    void setName(string name);
    string getName();
    Mesh* getMesh();
};

#endif