#ifndef GLB_FILE_HANDLER_C
#define GLB_FILE_HANDLER_C

#include <string>

class GlbFileHandler
{
private:
    static GlbFileHandler* _instance;

public:
    static GlbFileHandler* getInstance();
    void loadGlbFile(std::string path);
};

#endif