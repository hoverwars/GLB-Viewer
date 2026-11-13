#include "node.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

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

glm::mat4 Node::applyTransformations(glm::mat4 model)
{
    glm::mat4 local = glm::mat4(1.0f);

    local = glm::translate(local, _translation);
    glm::quat quaternion = glm::quat(_rotation.w, _rotation.x, _rotation.y, _rotation.z);
    local = local * glm::mat4_cast(quaternion);
    local = glm::scale(local, _scale);

    return _father == nullptr ? 
        model * local : 
        _father->applyTransformations(model) * local;
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

void Node::setFather(Node* father)
{
    _father = father;
}

bool Node::hasChild()
{
    return _children.size() != 0;
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

Mesh* Node::getMesh()
{
    return _mesh;
}