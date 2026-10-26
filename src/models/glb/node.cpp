#include "node.h"

Node::Node(string name, vector<Node*> children, glm::vec4 rotation, glm::vec3 scale, glm::vec3 translation)
{
    _name = name;
    _children = children;
    _mesh = nullptr;
    _rotation = rotation;
    _scale = scale;
    _translation = translation;
}

Node::Node(string name, Mesh* mesh, glm::vec4 rotation, glm::vec3 scale, glm::vec3 translation)
{
    _name = name;
    _children = vector<Node*>();
    _mesh = mesh;
    _rotation = rotation;
    _scale = scale;
    _translation = translation;
}

bool Node::hasChild(Node* node)
{
    for (Node* child : _children)
    {
        if (node == child)
        {
            return true;
        }
    }

    return false;
}

bool Node::hasChild()
{
    return _children.size() == 0;
}

vector<Node*> Node::getChildren()
{
    return _children;
}

void Node::setName(string name)
{
    _name = name;
}

string Node::getName()
{
    return _name;
}