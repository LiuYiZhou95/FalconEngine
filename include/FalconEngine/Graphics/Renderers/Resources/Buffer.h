#pragma once

#include <FalconEngine/Graphics/GraphicsInclude.h>

namespace FalconEngine
{

enum class BufferAccessMode
{
    None,

    Read,
    ReadWrite,
    Write,

    Count
};

enum class BufferUsage
{
    None,

    Static,
    Stream,
    Dynamic,

    Count
};

class Buffer : public Object
{
    FALCON_ENGINE_RTTI_DECLARE;

protected:
    /************************************************************************/
    /* Constructors and Destructor                                          */
    /************************************************************************/
    Buffer(int elementNum, int elementByteNum, BufferUsage usage);
    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

public:
    virtual ~Buffer();

public:
    unsigned char *GetData()
    {
        return mData;
    }

    const unsigned char *GetData() const
    {
        return mData;
    }

    int GetDataByteNum() const
    {
        return mDataByteNum;
    }

    int GetElementNum() const
    {
        return mElementNum;
    }

    void
    SetElementNum(int elementNum)
    {
        mElementNum = elementNum;
    }

    BufferUsage GetUsage() const
    {
        return mUsage;
    }

private:
    unsigned char *mData;
    int            mDataByteNum;
    int            mElementNum;
    BufferUsage    mUsage;
};

}
