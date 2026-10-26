#include <stdexcept>

#include "bufferViewImplementations.h"

// Scalar implementations

template <typename T>
ScalarBufferView<T>::ScalarBufferView(unsigned char* bytes, unsigned long length) : BufferView<T, T>(bytes, length)
{ }

template <typename T>
T ScalarBufferView<T>::buildContainer(T values[], unsigned __int8 length)
{
    if (length != 1) throw std::runtime_error("Cannot deal with the provided buffer size");
    return values[0];
}

template class ScalarBufferView<char>;
template class ScalarBufferView<unsigned char>;
template class ScalarBufferView<short>;
template class ScalarBufferView<unsigned short>;
template class ScalarBufferView<unsigned int>;
template class ScalarBufferView<float>;


// Vector 3d implementations

template <typename T>
Vector3dBufferView<T>::Vector3dBufferView(unsigned char* bytes, unsigned long length) : BufferView<glm::vec3, T>(bytes, length, 3)
{ }

template <typename T>
glm::vec3 Vector3dBufferView<T>::buildContainer(T values[], unsigned __int8 length)
{
    if (length != 3) throw std::runtime_error("Cannot deal with the provided buffer size");
    return glm::vec3(values[0], values[1], values[2]);
}

template class Vector3dBufferView<char>;
template class Vector3dBufferView<unsigned char>;
template class Vector3dBufferView<short>;
template class Vector3dBufferView<unsigned short>;
template class Vector3dBufferView<unsigned int>;
template class Vector3dBufferView<float>;