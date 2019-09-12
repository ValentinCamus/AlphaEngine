#include "Texture.h"

#include <Alpha/Engine/Renderer/Renderer.h>
#include <Alpha/Engine/Renderer/OpenGL/OpenGLTexture.h>

namespace Alpha
{

    Pointer<Texture2D> Texture2D::Create(const std::string &path)
    {
        switch (Renderer::GetRendererAPI())
        {
            case ERendererAPI::OpenGL:
                return NewPointer<OpenGLTexture2D>(path);
            default:
                break;
        }
        ALPHA_ASSERT(false, "Texture2D::Create: Undefined RendererAPI");
        return nullptr;
    }
}

