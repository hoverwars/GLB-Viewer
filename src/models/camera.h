#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Camera
{
private:
    static bool _isSceneHovered; 

    float _yaw = 0.0f;
    float _pitch = 35.0f;
    float _distance = 30.0f;

    bool _isDragging = false;
    glm::vec3 _target = glm::vec3(0.0f);
    static float _scrollDelta; // Accumulated mouse wheel since last frame

public:
    Camera();
    glm::mat4 updateCamera(float viewportHeight);
    static void updateZoom(GLFWwindow* window, double xOffset, double yOffset);
    static void setSceneHovered(bool sceneHovered);

    const float K_FOV_DEGREES;
    const float K_MIN_DISTANCE;
    const float K_MAX_DISTANCE;
};

#endif