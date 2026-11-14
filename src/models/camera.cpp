#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <cmath>
#include <imgui/imgui.h>

#include "../controllers/windowContextProvider.h"

bool Camera::_isSceneHovered = false;
float Camera::_scrollDelta = 0;

Camera::Camera() : K_FOV_DEGREES(45.0f), K_MIN_DISTANCE(1.0f), K_MAX_DISTANCE(200.0f)
{
    WindowContextProvider::getInstance()->setScrollCallback(Camera::updateZoom);
}

glm::mat4 Camera::updateCamera(float viewportHeight)
{
    ImGuiIO& io = ImGui::GetIO();
    const glm::vec3 worldUp(0.0f, 0.0f, 1.0f);

    // Orientation derived from yaw/pitch: always unit length, no manual normalization.
    auto computeFront = [this]() {
        const float yaw = glm::radians(_yaw);
        const float pitch = glm::radians(_pitch);
        // Camera sits at target + distance * dir, and looks back at the target.
        return -glm::vec3(cos(pitch) * cos(yaw), cos(pitch) * sin(yaw), sin(pitch));
    };

    // A drag starts only when the scene is hovered, and continues even if the cursor leaves it.
    if (_isSceneHovered && ImGui::IsMouseClicked(ImGuiMouseButton_Middle))
        _isDragging = true;
    if (!ImGui::IsMouseDown(ImGuiMouseButton_Middle))
        _isDragging = false;

    if (_isDragging)
    {
        // Moving the camera
        if (io.KeyShift)
        {
            // Pan: move the target along the camera's right/up axes.
            const glm::vec3 front = computeFront();
            const glm::vec3 right = glm::normalize(glm::cross(front, worldUp));
            const glm::vec3 camUp = glm::cross(right, front);

            // World size of one pixel at the target's depth -> the scene follows the cursor exactly.
            const float unitsPerPixel = 2.0f * _distance * tan(glm::radians(K_FOV_DEGREES) / 2.0f) / viewportHeight;
            _target += (-io.MouseDelta.x * right + io.MouseDelta.y * camUp) * unitsPerPixel;
        }
        // Rotating the camera
        else
        {
            // Orbit around the target.
            _yaw   -= io.MouseDelta.x * 0.4f;
            _pitch  = glm::clamp(_pitch + io.MouseDelta.y * 0.4f, -89.0f, 89.0f);
        }
    }

    // Zoom: multiplicative, so each notch feels the same at any distance.
    if (_scrollDelta != 0.0f)
    {
        _distance = glm::clamp(_distance * static_cast<float>(pow(0.9f, _scrollDelta)), K_MIN_DISTANCE, K_MAX_DISTANCE);
        _scrollDelta = 0.0f;
    }

    const glm::vec3 front = computeFront();
    const glm::vec3 position = _target - front * _distance;
    return glm::lookAt(position, _target, worldUp);
}

void Camera::resetPosition()
{
    _target = glm::vec3(0.0f);
}

bool Camera::isTargetingCenter()
{
    return _target == glm::vec3(0.0f);
}

void Camera::updateZoom(GLFWwindow* window, double xOffset, double yOffset)
{
    if (_isSceneHovered)
        _scrollDelta += (float)yOffset;
}

void Camera::setSceneHovered(bool sceneHovered)
{
    _isSceneHovered = sceneHovered;
}