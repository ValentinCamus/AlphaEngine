#include "Framebuffer.h"

#include <Alpha/Engine/Renderer/Renderer.h>
#include <Alpha/Engine/Renderer/OpenGL/OpenGLFramebuffer.h>

namespace Alpha
{

    Pointer<Framebuffer> Framebuffer::Create(uint32 width, uint32 height, EFormat fmt)
    {
        switch (Renderer::GetRendererAPI())
        {
            case ERendererAPI::OpenGL:
                return NewPointer<OpenGLFramebuffer>(width, height, fmt);
            default:
                break;
        }
        ALPHA_ASSERT(false, "Framebuffer::Create: Undefined RendererAPI");
        return nullptr;
    }
}