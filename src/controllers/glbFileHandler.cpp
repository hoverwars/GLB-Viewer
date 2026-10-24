#include <fstream>
#include <stdint.h>
#include <iostream>
#include <format>
#include <nlohmann/json.hpp>

#include "glbFileHandler.h"
#include "../utils/logs.h"
#include "../models/glb/glbFile.h"
#include "modelHandler.h"

using namespace std;
using json = nlohmann::json;

GlbFileHandler* GlbFileHandler::_instance = nullptr;

GlbFileHandler* GlbFileHandler::getInstance()
{
    if (GlbFileHandler::_instance == nullptr)
    {
        GlbFileHandler::_instance = new GlbFileHandler();
    }

    return GlbFileHandler::_instance;
}

void GlbFileHandler::loadGlbFile(string path)
{
    fstream glbFile(path, ios::in | ios::binary);

    if (!glbFile.is_open())
    {
        logError("Cannot open file at provided path: " + path);
        return;
    }

    uint32_t magic, version, length;

    glbFile.read((char*)&magic, sizeof(magic));
    glbFile.read((char*)&version, sizeof(version));
    glbFile.read((char*)&length, sizeof(length));

    if (version != 2)
    {
        logError(format("Cannot load provided model at path {0}. Unsupported GLTF version, {1} received, 2 expected.", path, version));
        return;
    }

    logInfo(format("Loading model located at {0}. GLB Version: {1}. Length: {2}", path, version, length));

    json structure;
    GlbFile* parsedFile;
    
    while (glbFile.peek() != EOF)
    {
        uint32_t chunkLength, chunkType;

        glbFile.read((char*)&chunkLength, sizeof(chunkLength));
        glbFile.read((char*)&chunkType, sizeof(chunkType));

        // JSON
        if (chunkType == 0x4E4F534A)
        {   
            char* chunkContent = new char[chunkLength + 1];
            glbFile.read(chunkContent, chunkLength);
            chunkContent[chunkLength] = '\0';

            structure = json::parse(chunkContent);
            cout << structure.dump(2) << endl;

            delete[] chunkContent;
        }
        // Binary asset
        else if (chunkType == 0x004E4942)
        {
            unsigned char chunkContent[chunkLength + 1];
            glbFile.read((char*)chunkContent, chunkLength);

            // According to the GLTF 2.0 specification, binary chunk must be the last chunk.
            parsedFile = new GlbFile(structure, chunkContent, chunkLength + 1);
            ModelHandler::getInstance()->addBaseFile(parsedFile);
        }
        else
        {
            logWarning("Unrecognized chunk type while reading model" + path);
        }
    }
}