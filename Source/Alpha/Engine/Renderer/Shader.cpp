#include "Shader.h"

#include <Alpha/Engine/Renderer/Renderer.h>

#include <Alpha/Engine/Renderer/OpenGL/OpenGLShader.h>

namespace Alpha
{
    const int32 Shader::GLSL_VERTEX_SHADER = GL_VERTEX_SHADER;
    const int32 Shader::GLSL_FRAGMENT_SHADER = GL_FRAGMENT_SHADER;
    const int32 Shader::GLSL_GEOMETRY_SHADER = GL_GEOMETRY_SHADER;

    Pointer<Shader> Shader::Create(const std::string& name, const std::map<int32, std::string>& sources)
    {
        switch (Renderer::GetRendererAPI())
        {
            case ERendererAPI::OpenGL:  return NewPointer<OpenGLShader>(name, sources);
            default: break;
        }

        ALPHA_ASSERT(false, "Shader::Create: Cannot create new shader: Invalid renderer API");
        return nullptr;
    }
}