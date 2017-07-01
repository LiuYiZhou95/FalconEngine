#include <FalconEngine/Graphics/Renderer/Resource/Buffer.h>

namespace FalconEngine
{
/************************************************************************/
/* Constructors and Destructor                                          */
/************************************************************************/
// @param elementNum - the element number
// @param elementByteNum - the element size in total.
Buffer::Buffer(int elementNum,
               size_t elementByteNum,
               BufferStorageMode storageMode,
               BufferType type,
               BufferUsage usage) :
    mDataOffset(0),
    mElementSize(elementByteNum),
    mStorageMode(storageMode),
    mType(type),
    mUsage(usage)
{
    if (elementNum < 1)
    {
        FALCON_ENGINE_THROW_RUNTIME_EXCEPTION("Invalid element number.");
    }

    // NOTE(Wuxiang): The capacity is initially determined by allocation at construction.
    mCapacityElementNum = elementNum;

    // NOTE(Wuxiang): The capacity is fixed currently.
    mCapacitySize = size_t(elementNum) * elementByteNum;
    mDataSize = mCapacitySize;

    mElementNum = elementNum;

    // NOTE(Wuxiang): Only allocate memory when the buffer storage resides on RAM.
    if (mStorageMode == BufferStorageMode::Host)
    {
        mData = new unsigned char[mDataSize];
    }
    else
    {
        mData = nullptr;
    }
}

Buffer::~Buffer()
{
    delete [] mData;
}

/************************************************************************/
/* Public Members                                                       */
/************************************************************************/
size_t
Buffer::GetCapacitySize() const
{
    return mCapacitySize;
}

unsigned char *
Buffer::GetData()
{
    return mData;
}

const unsigned char *
Buffer::GetData() const
{
    return mData;
}

size_t
Buffer::GetDataSize() const
{
    return mDataSize;
}

size_t
Buffer::GetDataOffset() const
{
    return mDataOffset;
}

void
Buffer::SetDataOffset(size_t dataOffset)
{
    mDataOffset = dataOffset;
}

int
Buffer::GetElementNum() const
{
    return mElementNum;
}

void
Buffer::SetElementNum(int elementNum)
{
    assert(elementNum >= 0);

    mElementNum = elementNum;
    mDataSize = size_t(mElementSize) * mElementNum;
}

int
Buffer::GetElementSize() const
{
    return mElementSize;
}

int
Buffer::GetElementOffset() const
{
    return mDataOffset / mElementSize;
}

BufferStorageMode
Buffer::GetStorageMode() const
{
    return mStorageMode;
}

BufferType
Buffer::GetType() const
{
    return mType;
}

BufferUsage
Buffer::GetUsage() const
{
    return mUsage;
}

}
