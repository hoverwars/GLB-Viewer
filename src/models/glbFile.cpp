#include "glbFile.h"
using json = nlohmann::json;

GlbFile::GlbFile(json structure, unsigned char* binary, unsigned int length)
{
    _structure = structure;

    if (length <= 0) return;

    parseBinary();
}

void GlbFile::parseBinary()
{
    
}