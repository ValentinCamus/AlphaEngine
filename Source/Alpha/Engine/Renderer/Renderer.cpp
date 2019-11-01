#include "Renderer.h"

#include <Alpha/Engine/Renderer/OpenGL/OpenGLRenderer.h>

namespace Alpha
{
    Renderer* Renderer::s_instance = nullptr;

    Pointer<Renderer::DrawOptions> Renderer::s_options = nullptr;

    ERendererAPI Renderer::s_rendererAPI = ERendererAPI::OpenGL;

    void Renderer::Init()
    {
        s_options = NewPointer<Renderer::DrawOptions>();

        switch (Renderer::GetRendererAPI())
        {
            case ERendererAPI::OpenGL:
                Renderer::s_instance = new OpenGLRendererAPI();
                break;
            default:
                Renderer::s_instance = nullptr;
                break;
        }

        ALPHA_ASSERT(s_instance, "Invalid Renderer!");

        s_instance->InitImpl();
    }
}