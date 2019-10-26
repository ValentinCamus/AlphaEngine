#include "OpenGLStaticMesh.h"

#include <Alpha/Engine/Renderer/OpenGL/OpenGLRenderer.h>

namespace Alpha
{

    OpenGLStaticMesh::OpenGLStaticMesh(const std::vector<Vertex> &vertices,
                                       const std::vector<uint32> &indices)
            : m_vertices(vertices)
            , m_indices(indices)
    {
        SetupMesh();
    }

    void OpenGLStaticMesh::Destroy()
    {
        GL_CHECK(glDeleteBuffers(1, &m_ibo));
        GL_CHECK(glDeleteBuffers(1, &m_vbo));
        GL_CHECK(glDeleteVertexArrays(1, &m_vao));
    }

    void OpenGLStaticMesh::Draw(const Pointer<Shader> &shader, const TransformMatrix &transform, EDrawMode drawMode)
    {
        bool bIsMaterialDisable = Renderer::IsDisable(Renderer::EOption::DiscardMaterial);

        if (!IsMaterialValid() && bIsMaterialDisable)
        {
            Logger::Warn("OpenGLStaticMesh::Draw No Material specified, the object will not be rendered.");
        }
        else
        {
            shader->SetUniform("model", transform.model);
            shader->SetUniform("view", transform.view);
            shader->SetUniform("proj", transform.projection);

            GL_CHECK(glBindVertexArray(m_vao));
            GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo));


            if (bIsMaterialDisable) GetMaterial()->Bind(shader, "material");
            GL_CHECK(glDrawElements(CastDrawMode(drawMode), (GLsizei) m_indices.size(), GL_UNSIGNED_INT, nullptr));
            if (bIsMaterialDisable) GetMaterial()->Unbind();


            GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
            GL_CHECK(glBindVertexArray(0));
        }

    }

    void OpenGLStaticMesh::SetupMesh()
    {
        void * offsetOf = nullptr;
        auto iboSizeInByte = static_cast<uint32>(m_indices.size() * sizeof(uint32));
        auto vboSizeInByte = static_cast<uint32>(m_vertices.size() * sizeof(Vertex));

        GL_CHECK(glGenVertexArrays(1, &m_vao));
        GL_CHECK(glGenBuffers(1, &m_vbo));
        GL_CHECK(glGenBuffers(1, &m_ibo));

        GL_CHECK(glBindVertexArray(m_vao));
        GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));

        GL_CHECK(glBufferData(GL_ARRAY_BUFFER, vboSizeInByte, &m_vertices[0], GL_STATIC_DRAW));
        GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo));
        GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, iboSizeInByte, &m_indices[0], GL_STATIC_DRAW));

        GL_CHECK(glEnableVertexAttribArray(0));
        GL_CHECK(glEnableVertexAttribArray(1));
        GL_CHECK(glEnableVertexAttribArray(2));

        // Vertex positions
        offsetOf = (void*) offsetof(Vertex, position);
        GL_CHECK(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetOf));

        // Vertex normals
        offsetOf = (void*) offsetof(Vertex, normal);
        GL_CHECK(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetOf));

        // Vertex texture coords
        offsetOf = (void*) offsetof(Vertex, texCoords);
        GL_CHECK(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetOf));

        GL_CHECK(glBindVertexArray(0));
    }

    GLenum OpenGLStaticMesh::CastDrawMode(EDrawMode drawMode) const
    {
        switch (drawMode)
        {
            case EDrawMode::Points: return GL_POINTS;
            case EDrawMode::Lines: return GL_LINES;
            case EDrawMode::Triangles: return GL_TRIANGLES;
        }
    }
}