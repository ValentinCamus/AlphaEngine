#include "Renderer.h"

#include <Alpha/Engine/Renderer/OpenGL/OpenGLRenderer.h>

namespace Alpha
{
    Renderer* Renderer::s_instance = nullptr;

    ERendererAPI Renderer::s_rendererAPI = ERendererAPI::OpenGL;

    bool Renderer::s_bDiscardMaterial = false;

    void Renderer::Init()
    {
        switch (Renderer::GetRendererAPI())
        {
            case ERendererAPI::OpenGL:
                Renderer::s_instance = new OpenGLRendererAPI();
                break;
            default:
                Renderer::s_instance = nullptr;
                break;
        }

        ALPHA_ASSERT(s_instance, "Invalid renderer!");
        s_instance->InitImpl();
    }
}