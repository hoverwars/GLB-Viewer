#include "convert.h"

#include <stdexcept>

using namespace std;

glm::vec4 toGlmVector(ImVec4 color)
{
    return glm::vec4(color.x, color.y, color.z, color.w);
}

glm::vec4 toGlmVector(vector<float> color)
{
    if (color.size() != 4) throw runtime_error("Invalid vector size: Must be of size 4.");

    return glm::vec4(color[0], color[1], color[2], color[3]);
}