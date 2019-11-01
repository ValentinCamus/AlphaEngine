#pragma once

#include <Alpha/Core/CoreMinimal.h>

#include <Alpha/Engine/Renderer/StaticMesh.h>

#include <Alpha/Engine/Renderer/OpenGL/OpenGLTools.h>
#include <Alpha/Engine/Renderer/OpenGL/OpenGLRenderer.h>

namespace Alpha
{
    class OpenGLStaticMesh : public StaticMesh
    {
    public:
        /// Load the mesh from data.
        explicit OpenGLStaticMesh(const std::vector<Vertex>& vertices, const std::vector<uint32>& indices);

        ~OpenGLStaticMesh() override { Destroy(); }

        /// Destroy the mesh.
        void Destroy() override;

        /// Render the mesh.
        void Draw(const Pointer<Shader>& shader, const TransformMatrix& transform) override;

        /// @getter: The mesh's vertices
        inline const std::vector<Vertex>& GetVertices() const override { return m_vertices; }
        /// @getter: The mesh's indices
        inline const std::vector<uint32>& GetIndices() const override { return m_indices; }

        /// @getter: The mesh's id.
        inline int32 GetId() const override { return m_vao; }

    private:
        void SetupMesh();

        GLenum CastDrawMode(Renderer::EDrawMode drawMode) const;

    private:
        /// The mesh's vertices.
        std::vector<Vertex> m_vertices;

        /// The mesh's indices.
        std::vector<uint32> m_indices;

        /// Vertex Array Object Id.
        uint32 m_vao = 0;

        /// Vertex Buffer Object Id.
        uint32 m_vbo = 0;

        /// Indices Array Object Id.
        uint32 m_ibo = 0;
    };
}