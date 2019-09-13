#include "Shader.h"

#include <Alpha/Engine/Renderer/Renderer.h>

#include <Alpha/Engine/Renderer/OpenGL/OpenGLShader.h>

namespace Alpha
{
    Pointer<Shader> Shader::Create(const std::string& filepath)
    {
        switch (Renderer::GetRendererAPI())
        {
            case ERendererAPI::OpenGL:  return NewPointer<OpenGLShader>(filepath);
            default: break;
        }

        ALPHA_ASSERT(false, "Shader::Create: Cannot create new shader: Invalid renderer API");
        return nullptr;
    }
}