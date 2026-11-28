#include <glm/glm.hpp>
#include <vector>
#include <imgui/imgui.h>

using namespace std;

glm::vec4 toGlmVector(ImVec4 color);
glm::vec4 toGlmVector(vector<float> color);