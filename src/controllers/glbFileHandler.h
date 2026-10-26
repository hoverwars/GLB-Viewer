#ifndef GLB_FILE_HANDLER_C
#define GLB_FILE_HANDLER_C

#include <string>
#include <vector>

#include "../models/glb/glbFile.h"

using namespace std;

class GlbFileHandler
{
private:
    static GlbFileHandler* _instance;
    vector<GlbFile*> _loadedFiles;

public:
    static GlbFileHandler* getInstance();
    GlbFile* loadGlbFile(string path);
    ~GlbFileHandler();
};

#endif