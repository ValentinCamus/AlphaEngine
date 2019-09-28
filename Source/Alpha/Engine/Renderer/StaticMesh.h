#pragma once

#include <Alpha/Core/CoreMinimal.h>
#include <Alpha/Engine/Renderer/Renderer.h>

#include <Alpha/Engine/Renderer/Shader.h>
#include <Alpha/Engine/Renderer/Texture.h>

#include <Alpha/Engine/Material/Material.h>

namespace Alpha
{
    class StaticMesh
    {
    public:
        /// Load the mesh from raw data.
        static Pointer<StaticMesh> Create(const std::vector<Vertex>& vertices, const std::vector<uint32>& indices);

    public:
        virtual ~StaticMesh() = default;

        /// Destroy the mesh.
        virtual void Destroy() = 0;

        /// Render the mesh.
        virtual void Draw(const Pointer<Shader>& shader, const TransformMatrix& transform, EDrawMode drawMode) = 0;

        /// @getter: The mesh's vertices
        virtual const std::vector<Vertex>& GetVertices() const = 0;
        /// @getter: The mesh's indices
        virtual const std::vector<uint32>& GetIndices() const = 0;

        /// @getter: The mesh's id.
        virtual int32 GetId() const = 0;

    public:
        /// @getter: The mesh's material.
        virtual inline const Pointer<Material>& GetMaterial() { return m_material; }
        /// @setter: The mesh's material.
        virtual inline void SetMaterial(const Pointer<Material>& m) { m_material = m; }
        /// @getter: Check if the material is valid.
        virtual inline bool IsMaterialValid() { return m_material != nullptr; }

    private:
        /// The mesh's material.
        Pointer<Material> m_material = nullptr;
    };
}