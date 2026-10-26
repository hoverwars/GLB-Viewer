#include <format>

#include "nodeHandler.h"

using namespace std;

NodeHandler* NodeHandler::_instance = nullptr;

NodeHandler::NodeHandler()
{
    _nodes = vector<Node*>();
    //_models = vector<Model*>();
}

NodeHandler::~NodeHandler()
{
    for (Node* node : _nodes)
    {
        delete node;
    }
}

NodeHandler* NodeHandler::getInstance()
{
    if (_instance == nullptr)
    {
        _instance = new NodeHandler();
    }

    return _instance;
}

string NodeHandler::getUniqueName(Node* node)
{
    string desiredName = node->getName();
    string choosenName = desiredName;
    int counter = 0;
    bool alreadyExists = false;

    do 
    {
        if (alreadyExists)
        {
            counter++;
            choosenName = format("{}_{}", desiredName, counter);
            alreadyExists = false; 
        }

        for (Node* existing : _nodes)
        {
            if (existing->getName() == choosenName)
            {
                alreadyExists = true;
                break;
            }
        }
    }
    while (alreadyExists);

    return choosenName;
}

void NodeHandler::loadNodes(GlbFile* baseFile)
{
    // Get raw nodes from the glb file. Will do some processing on them.
    vector<Node*> rawNodes = baseFile->getNodes();
    vector<Node*> nodes{};

    // Firstly, remove nodes that are referenced by parents.
    for (Node* node : rawNodes)
    {
        bool referenced = false;

        for (Node* other : rawNodes)
        {
            if (node != other && other->hasChild(node))
            {
                referenced = true;
                break;
            }
        }

        if (!referenced)
        {
            nodes.push_back(node);
        }
    }

    // Insert each resulting node in the list of node handled by the application, changing the name if needed
    for (Node * node : nodes)
    {
        string name = getUniqueName(node);
        if (name != node->getName())
        {
            node->setName(name);
        }

        _nodes.push_back(node);
    }
}