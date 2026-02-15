#pragma once

#include <vector>
#include <cstring>
#include "Log.h"

struct Buffer
{
    void *Data;
    size_t Size;

    Buffer(void *data, size_t size)
        : Data(data), Size(size)
    {}

    Buffer(Buffer &b)
        : Data(b.Data), Size(b.Size)
    {}
    Buffer(Buffer &&b)
        : Data(b.Data), Size(b.Size)
    {}

    void Release(void)
    {
        DBG_LOG("");
        delete Data;
        Size = 0;
    }
};

struct OwnedBuffer : public Buffer
{
    OwnedBuffer(size_t size)
        : Buffer(new char[size], size)
    {
        DBG_LOG("Allocted %zu", size);
    }
    ~OwnedBuffer()
    {
        Release();
    }

    OwnedBuffer& OwnedBuffer::operator =(const OwnedBuffer& b);
    OwnedBuffer& OwnedBuffer::operator =(const std::vector<unsigned char> &v)
    {
        DBG_LOG("");
        size_t n = v.size();
        Size = n;
        if (n > Size)
            Release();
            Data = new char[n];
        memcpy(Data, v.data(), n);
        return *this;
    }
};