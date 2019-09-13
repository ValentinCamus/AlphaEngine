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
        glDeleteBuffers(1, &m_ibo);
        glDeleteBuffers(1, &m_vbo);
        glDeleteVertexArrays(1, &m_vao);
    }

    void OpenGLStaticMesh::Draw(const Pointer<Shader> &shader, const TransformMatrix &transform)
    {
        if (!IsMaterialValid())
        {
            Logger::Warn("OpenGLStaticMesh::Draw No Material specified, the object will not be rendered.");
        }
        else
        {
            shader->SetUniform("transform", transform);

            glBindVertexArray(m_vao);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
            GetMaterial()->Bind(shader);

            glDrawElements(GL_TRIANGLES, (GLsizei) m_indices.size(), GL_UNSIGNED_INT, nullptr);

            GetMaterial()->Unbind();
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

    }

    void OpenGLStaticMesh::SetupMesh()
    {
        void * offsetOf = nullptr;
        uint32 iboSizeInByte = m_indices.size() * sizeof(uint32);
        uint32 vboSizeInByte = m_vertices.size() * sizeof(Vertex);

        glGenVertexArrays(1, &m_vao);
        glGenBuffers(1, &m_vbo);
        glGenBuffers(1, &m_ibo);

        glBindVertexArray(m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

        glBufferData(GL_ARRAY_BUFFER, vboSizeInByte, &m_vertices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, iboSizeInByte, &m_indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        // Vertex positions
        offsetOf = (void*) offsetof(Vertex, position);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetOf);

        // Vertex normals
        offsetOf = (void*) offsetof(Vertex, normal);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetOf);

        // Vertex texture coords
        offsetOf = (void*) offsetof(Vertex, texCoords);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetOf);

        glBindVertexArray(0);
    }
}