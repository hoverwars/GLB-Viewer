#ifndef BUFFER_VIEW_H
#define BUFFER_VIEW_H

#include <vector>
#include <cstring>

using namespace std;

// Building an int exactly of the right size, because some BaseType generic (!= int) does not support |= operator
template <size_t N> struct AjustedUInt;
template <> struct AjustedUInt<1> { using type = uint8_t;  };
template <> struct AjustedUInt<2> { using type = uint16_t; };
template <> struct AjustedUInt<4> { using type = uint32_t; };

template<typename ContainerType, typename BaseType> 
class BufferView
{
private:
    unsigned char* _bytes;
    unsigned long _length;
    unsigned __int8 _containerSize;
    vector<ContainerType> _data;

    virtual BaseType buildBaseValue(unsigned char* bytes, short length);
    virtual ContainerType buildContainer(BaseType values[], unsigned __int8 length) = 0;

protected:
    BufferView(unsigned char* bytes, unsigned long length, unsigned __int8 containerSize);
    BufferView(unsigned char* bytes, unsigned long length);
    
public:
    inline vector<ContainerType> getData() { return _data; }
    void parse();
};

// Implementations

template<typename ContainerType, typename BaseType>
BufferView<ContainerType, BaseType>::BufferView(unsigned char* bytes, unsigned long length, unsigned __int8 containerSize)
{
    _bytes = bytes;
    _length = length;
    _containerSize = containerSize;
    _data = vector<ContainerType>();
}

template<typename ContainerType, typename BaseType>
BufferView<ContainerType, BaseType>::BufferView(unsigned char* bytes, unsigned long length) : BufferView(bytes, length, 1)
{ }

template<typename ContainerType, typename BaseType>
BaseType BufferView<ContainerType, BaseType>::buildBaseValue(unsigned char* bytes, short length) 
{
    BaseType value{};

    using AjustedUInt = typename AjustedUInt<sizeof(BaseType)>::type;
    AjustedUInt bits = 0;
    for (short i = 0; i < length; i++)
    {
        bits |= (bytes[i] << (i * 8));
    }

    memcpy(&value, &bits, sizeof(BaseType));
    return value;
}

template<typename ContainerType, typename BaseType>
void BufferView<ContainerType, BaseType>::parse()
{
    if (_bytes == nullptr || _length == 0) throw runtime_error("Cannot parse buffer. Buffer content uninitialized.");

    short baseTypeSize = sizeof(BaseType);
    int valueCount = _length / baseTypeSize;  // Number of values expected
    int containerCount = valueCount / _containerSize; // Number of containers expected, meaning the size of _data attribute.
    long byteBufferReadHead = 0;

    for (int cc = 0; cc < containerCount; cc++)
    {
        BaseType values[_containerSize];
        for (int vc = 0; vc < valueCount; vc++)
        {
            unsigned char rawValue[baseTypeSize];
            for (int bts = 0; bts < baseTypeSize; bts++)
            {
                rawValue[bts] = _bytes[byteBufferReadHead];
                byteBufferReadHead++;
            }

            values[vc] = buildBaseValue(rawValue, baseTypeSize);
        }

        _data.push_back(buildContainer(values, _containerSize));
    }

    _bytes = nullptr;
    _length = 0;
}

#endif