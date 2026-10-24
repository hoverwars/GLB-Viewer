#ifndef GLB_FILE_H
#define GLB_FILE_H

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class GlbFile
{
private:
    json _structure;

    void parseBinary();

public:
    GlbFile(json structure, unsigned char* binary, unsigned int length);
};

#endif