#include "SkeletalMesh.h"

#include <Alpha/Engine/Renderer/Renderer.h>

#include <Alpha/Engine/Renderer/OpenGL/OpenGLSkeletalMesh.h.h>

namespace Alpha
{

    Pointer<SkeletalMesh> SkeletalMesh::Create(const std::vector<SkeletalMesh::Vertex>& vertices,
                                               const std::vector<uint32>& indices)
    {
        switch (Renderer::GetRendererAPI())
        {
            case ERendererAPI::OpenGL: return NewPointer<OpenGLSkeletalMesh>(vertices, indices);
            default: break;
        }

        ALPHA_ASSERT(false, "Shader::Create: Cannot create new skeletal mesh: Invalid renderer API");
        return nullptr;
    }
}