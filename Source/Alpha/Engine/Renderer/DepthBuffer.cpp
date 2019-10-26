#include "DepthBuffer.h"

#include <Alpha/Engine/Renderer/Renderer.h>
#include <Alpha/Engine/Renderer/OpenGL/OpenGLDepthBuffer.h>

namespace Alpha
{

    Pointer<DepthBuffer> DepthBuffer::Create(uint32 width, uint32 height)
    {
        switch (Renderer::GetRendererAPI())
        {
            case ERendererAPI::OpenGL:
                return NewPointer<OpenGLDepthBuffer>(width, height);
            default:
                break;
        }
        ALPHA_ASSERT(false, "DepthBuffer::Create: Undefined RendererAPI");
        return nullptr;
    }
}