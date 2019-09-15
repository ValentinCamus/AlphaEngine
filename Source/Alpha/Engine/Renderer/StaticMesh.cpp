#include "StaticMesh.h"

#include <Alpha/Engine/Renderer/Renderer.h>

#include <Alpha/Engine/Renderer/OpenGL/OpenGLStaticMesh.h>

namespace Alpha
{

    Pointer<StaticMesh> StaticMesh::Create(const std::vector<Vertex> &vertices, const std::vector<uint32> &indices)
    {
        switch (Renderer::GetRendererAPI())
        {
            case ERendererAPI::OpenGL: return NewPointer<OpenGLStaticMesh>(vertices, indices);
            default: break;
        }

        ALPHA_ASSERT(false, "Shader::Create: Cannot create new static mesh: Invalid renderer API");
        return nullptr;
    }
}