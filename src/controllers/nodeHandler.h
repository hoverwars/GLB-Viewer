#ifndef MODEL_HANDLER_H
#define MODEL_HANDLER_H

#include <vector>
#include <string>

#include "../models/glb/node.h"
#include "../models/glb/glbFile.h"

using namespace std;

class NodeHandler
{
private:
    static NodeHandler* _instance;
    vector<Node*> _nodes;

    NodeHandler();
    string getUniqueName(Node* node);
    void collectSelectedRoots(Node* node, vector<Node*>& result);

public:
    static NodeHandler* getInstance();
    void loadNodes(GlbFile* baseFile);
    vector<Node*> getNodes();
    // Selected nodes whose father is not selected (transforming a father already moves its children).
    vector<Node*> getSelectedRoots();
    ~NodeHandler();
};

#endif