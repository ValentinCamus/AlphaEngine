#include "Cubemap.h"

#include <Alpha/Engine/Renderer/Renderer.h>
#include <Alpha/Engine/Renderer/OpenGL/OpenGLCubemap.h>

namespace Alpha
{

    Pointer<Cubemap> Cubemap::Create(const std::vector<std::string> &paths)
    {
        switch (Renderer::GetRendererAPI())
        {
            case ERendererAPI::OpenGL:
                return NewPointer<OpenGLCubemap>(paths);
            default:
                break;
        }
        ALPHA_ASSERT(false, "Texture2D::Create: Undefined RendererAPI");
        return nullptr;
    }
}