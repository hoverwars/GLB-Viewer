#ifndef MODEL_HANDLER_H
#define MODEL_HANDLER_H

#include <vector>

#include "../models/glb/glbFile.h"
#include "../models/glb/mesh.h"

using namespace std;

class ModelHandler
{
private:
    static ModelHandler* _instance;

    vector<GlbFile*> _glbBases;

    ModelHandler();

public:
    static ModelHandler* getInstance();
    void addBaseFile(GlbFile* baseFile);
    ~ModelHandler();
};

#endif