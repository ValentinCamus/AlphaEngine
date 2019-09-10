#include "Renderer.h"

#include <Alpha/Engine/Renderer/OpenGL/OpenGLRenderer.h>

namespace Alpha
{
    Renderer* Renderer::s_instance = new OpenGLRendererAPI();
    ERendererAPI Renderer::s_rendererAPI = ERendererAPI::OpenGL;
}