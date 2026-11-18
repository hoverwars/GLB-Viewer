#include "node.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <sstream>

Node::Node(string name, vector<Node*> children, glm::vec4 rotation, glm::vec3 scale, glm::vec3 translation)
{
    _name = name;
    _children = children;
    _mesh = nullptr;
    _rotation = rotation;
    _scale = scale;
    _translation = translation;
    _isVisible = true;

    std::stringstream ss;
    ss << (void const *)this;
    _uniqueId = ss.str();
}

Node::Node(string name, Mesh* mesh, glm::vec4 rotation, glm::vec3 scale, glm::vec3 translation)
{
    _name = name;
    _children = vector<Node*>();
    _mesh = mesh;
    _rotation = rotation;
    _scale = scale;
    _translation = translation;
    _isVisible = true;

    std::stringstream ss;
    ss << (void const *)this;
    _uniqueId = ss.str();
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

Node* Node::getFather()
{
    return _father;
}

glm::vec4 Node::getRotation()
{
    return _rotation;
}

glm::vec3 Node::getTranslation()
{
    return _translation;
}

glm::vec3 Node::getScale()
{
    return _scale;
}

void Node::setRotation(glm::vec4 rotation)
{
    _rotation = rotation;
}

void Node::setTranslation(glm::vec3 translation)
{
    _translation = translation;
}

void Node::setScale(glm::vec3 scale)
{
    _scale = scale;
}

string Node::getUniqueId()
{
    return _uniqueId;
}

bool Node::isVisible()
{
    if (_father == nullptr) return _isVisible;
    else return _father->isVisible() && _isVisible;
}

bool Node::isFatherVisible()
{
    return _father == nullptr || _father->isVisible();
}

void Node::switchVisibility()
{
    _isVisible = !_isVisible;
}

void Node::setSelected(bool selected)
{
    _selected = selected;
    for (Node* child : _children)
    {
        child->setSelected(selected);
    }
}

bool Node::isSelected()
{
    return _selected;
}