#ifndef GLB_FILE_H
#define GLB_FILE_H

#include <vector>
#include <nlohmann/json.hpp>

#include "buffer/bufferView.h"
#include "mesh.h"
#include "node.h"

using json = nlohmann::json;
using namespace std;

struct RawBufferView
{
    unsigned char* binary;
    int size;
};

class GlbFile
{
private:
    json _structure;
    vector<ABufferView*> _buffers;
    vector<Mesh*> _meshes;

    vector<RawBufferView*> _rawBuffers; // Internal variable used to keep raw buffer data while we need it.

    void parseBinary(unsigned char* binary, unsigned int length);

    // Method used by parseBinary
    void parseRawBuffers(unsigned char* binary);
    void parseAccessors(); // Convert raw buffers to actual buffers understood by the application.
    void parseMeshes();
    ABufferView* createBuffer(RawBufferView* rawBuffer, int componentType, string type);

public:
    GlbFile(json structure, unsigned char* binary, unsigned int length);
    ~GlbFile();
    vector<Node*> getNodes();
};

#endif