
#include "modelHandler.h"

using namespace std;

ModelHandler* ModelHandler::_instance = nullptr;

ModelHandler::ModelHandler()
{
    _glbBases = vector<GlbFile*>();
    //_models = vector<Model*>();
}

ModelHandler::~ModelHandler()
{
    for (GlbFile* base : _glbBases)
    {
        delete base;
    }

    /*for (Model* model : _models)
    {
        delete model;
    }*/
}

ModelHandler* ModelHandler::getInstance()
{
    if (_instance == nullptr)
    {
        _instance = new ModelHandler();
    }

    return _instance;
}

void ModelHandler::addBaseFile(GlbFile* baseFile)
{
    _glbBases.push_back(baseFile);

    // TODO : parse nodes of the glb files, create one model per node + assign name to model
    //_models.push_back(new Model());
}