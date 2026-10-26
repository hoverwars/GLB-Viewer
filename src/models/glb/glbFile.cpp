#include <format>
#include <stdexcept>

#include "glbFile.h"
#include "meshPart.h"
#include "mesh.h"
#include "buffer/bufferViewImplementations.h"
#include "../../utils/logs.h"

using json = nlohmann::json;
using namespace std;

GlbFile::GlbFile(json structure, unsigned char* binary, unsigned int length)
{
    _structure = structure;
    _buffers = vector<ABufferView*>();
    _rawBuffers = vector<RawBufferView*>();
    _meshes = vector<Mesh*>();

    if (length <= 0) return;

    parseBinary(binary, length);
}

GlbFile::~GlbFile()
{
    for (ABufferView* bufferView : _buffers)
    {
        delete bufferView;
    }

    for (Mesh* mesh : _meshes)
    {
        delete mesh;
    }

    // _rawBuffer yet cleared
}

vector<Node*> GlbFile::getNodes()
{
    vector<Node*> nodes;
    for (json nodeDesc : _structure["nodes"])
    {
        string name = nodeDesc.value("name", "Node");
        vector<int> childrenIds = nodeDesc.value("children", vector<int>{});
        int meshId = nodeDesc.value("mesh", -1);
        vector<float> rotation = nodeDesc.value("rotation", vector<float>{});
        vector<float> scale = nodeDesc.value("scale", vector<float>{});
        vector<float> translation = nodeDesc.value("translation", vector<float>{});

        if (childrenIds.size() > 0)
        {
            vector<Node*> children = vector<Node*>();
            
            for (int childId : childrenIds)
            {
                children.push_back(nodes[childId]);
            }

            nodes.push_back(new Node(
                name,
                children,
                rotation.size() != 4 ? glm::vec4(1.0f) : glm::vec4(rotation[0], rotation[1], rotation[2], rotation[3]),
                scale.size() != 3 ? glm::vec3(1.0f) : glm::vec3(scale[0], scale[1], scale[2]),
                translation.size() != 3 ? glm::vec3(1.0f) : glm::vec3(translation[0], translation[1], translation[2])
            ));
        }
        else if (meshId != -1)
        {
            Mesh* mesh = _meshes[meshId];

            nodes.push_back(new Node(
                name,
                mesh,
                rotation.size() != 4 ? glm::vec4(1.0f) : glm::vec4(rotation[0], rotation[1], rotation[2], rotation[3]),
                scale.size() != 3 ? glm::vec3(1.0f) : glm::vec3(scale[0], scale[1], scale[2]),
                translation.size() != 3 ? glm::vec3(1.0f) : glm::vec3(translation[0], translation[1], translation[2])
            ));
        }
    }

    return nodes;
}

void GlbFile::parseBinary(unsigned char* binary, unsigned int length)
{
    parseRawBuffers(binary);
    parseAccessors();
    parseMeshes();

    for (RawBufferView* rawBuffer : _rawBuffers)
    {
        delete [] rawBuffer->binary;
        delete rawBuffer;
    }
}

void GlbFile::parseAccessors()
{
    for (json accessor : _structure["accessors"])
    {
        RawBufferView* rawBuffer = _rawBuffers[accessor.value("bufferView", -1)];
        int componentType = accessor.value("componentType", -1);
        string type = accessor.value("type", "");

        ABufferView* bufferView = createBuffer(rawBuffer, componentType, type);
        if (bufferView != nullptr)
        {
            bufferView->parse();
            _buffers.push_back(bufferView);
        }
    }
}

void GlbFile::parseRawBuffers(unsigned char* binary)
{
    for (json bufferDesc : _structure["bufferViews"])
    {
        int bufferLength = bufferDesc.value("byteLength", -1);
        int bufferOffset = bufferDesc.value("byteOffset", -1);

        unsigned char* bufferContent = new unsigned char[bufferLength + 1];
        bufferContent[bufferLength] = '\0';
        for (int i = 0; i < bufferLength; i++)
        {
            bufferContent[i] = binary[i + bufferOffset];
        }

        _rawBuffers.push_back(new RawBufferView{ bufferContent, bufferLength });
    }
}

void GlbFile::parseMeshes()
{
    for (json meshDesc : _structure["meshes"])
    {
        vector<MeshPart*> meshParts;
        for (json meshPartDesc : meshDesc["primitives"])
        {
            ABufferView* indicesBuffer = _buffers[meshPartDesc.value("indices", -1)];
            ABufferView* verticesBuffer = _buffers[meshPartDesc["attributes"].value("POSITION", -1)];
        
            // Safe cast here, because an error would mean that the file is malformed
            meshParts.push_back(new MeshPart(
                (Vector3dBufferView<float>*) verticesBuffer,
                (ScalarBufferView<short>*) indicesBuffer
            ));
        }

        _meshes.push_back(new Mesh(meshDesc.value("name", ""), meshParts));
    }
}

ABufferView* GlbFile::createBuffer(RawBufferView* rawBuffer, int componentType, string type)
{
    if (type == "SCALAR")
    {
        switch (componentType)
        {
            case 5120: return new ScalarBufferView<char>(rawBuffer->binary, rawBuffer->size);
            case 5121: return new ScalarBufferView<unsigned char>(rawBuffer->binary, rawBuffer->size);
            case 5122: return new ScalarBufferView<short>(rawBuffer->binary, rawBuffer->size);
            case 5123: return new ScalarBufferView<unsigned short>(rawBuffer->binary, rawBuffer->size);
            case 5125: return new ScalarBufferView<unsigned int>(rawBuffer->binary, rawBuffer->size);
            case 5126: return new ScalarBufferView<float>(rawBuffer->binary, rawBuffer->size);
            default: throw runtime_error(format("Invalid component type provided: {}", componentType));
        }
    }
    else if (type == "VEC3")
    {
        switch (componentType)
        {
            case 5120: return new Vector3dBufferView<char>(rawBuffer->binary, rawBuffer->size);
            case 5121: return new Vector3dBufferView<unsigned char>(rawBuffer->binary, rawBuffer->size);
            case 5122: return new Vector3dBufferView<short>(rawBuffer->binary, rawBuffer->size);
            case 5123: return new Vector3dBufferView<unsigned short>(rawBuffer->binary, rawBuffer->size);
            case 5125: return new Vector3dBufferView<unsigned int>(rawBuffer->binary, rawBuffer->size);
            case 5126: return new Vector3dBufferView<float>(rawBuffer->binary, rawBuffer->size);
            default: throw runtime_error(format("Invalid component type provided: {}", componentType));
        }
    }
    else
    {
        logWarning("Unsupported buffer type provided: " + type);
        return nullptr;
    }
}