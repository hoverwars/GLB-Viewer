#ifndef BUFFER_VIEW_IMPLEMENTATIONS_H
#define BUFFER_VIEW_IMPLEMENTATIONS_H

#include <glm/glm.hpp>

#include "bufferView.h"

template <typename T>
class ScalarBufferView : public BufferView<T, T>
{
private:
    virtual T buildContainer(T values[], unsigned __int8 length);

public:
    ScalarBufferView(unsigned char* bytes, unsigned long length);
};

template <typename T>
class Vector3dBufferView : public BufferView<glm::vec3, T>
{
private:
    virtual glm::vec3 buildContainer(T values[], unsigned __int8 length);

public:
    Vector3dBufferView(unsigned char* bytes, unsigned long length);
};

#endif