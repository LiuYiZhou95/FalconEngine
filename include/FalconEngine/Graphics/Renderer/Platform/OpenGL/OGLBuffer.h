#pragma once

#include <FalconEngine/Graphics/Renderer/Platform/OpenGL/OGLMapping.h>

namespace FalconEngine
{

class FALCON_ENGINE_API PlatformBuffer
{
public:
    /************************************************************************/
    /* Constructors and Destructor                                          */
    /************************************************************************/
    explicit PlatformBuffer(GLuint target, const Buffer *buffer);
    virtual ~PlatformBuffer();

public:
    /************************************************************************/
    /* Public Members                                                       */
    /************************************************************************/
    // @summary Map buffer data in the memory, so you could copy data into
    // the buffer by yourself.
    //
    // @return Buffer memory pointer
    void *
    Map(BufferAccessMode          access,
        BufferFlushMode           flush,
        BufferSynchronizationMode synchronization,
        int64_t                   offset,
        int64_t                   size);

    // @summary Unmap buffer.
    void
    Unmap();

    // @remark Offset is relative to the start of the currently mapped range of buffer.
    void
    Flush(int64_t offset, int64_t size);

protected:
    /************************************************************************/
    /* Protected Members                                                    */
    /************************************************************************/
    void
    CheckRangeValid(int64_t offset, int64_t size);

    void
    PrintRange(const char *message, GLuint bufferTarget, int64_t offset, int64_t size);

private:
    /************************************************************************/
    /* Private Members                                                      */
    /************************************************************************/
    void
    Create();

protected:
    GLuint        mBufferObj;
    const Buffer *mBufferPtr;

private:
    GLuint        mBufferTarget;
};

}