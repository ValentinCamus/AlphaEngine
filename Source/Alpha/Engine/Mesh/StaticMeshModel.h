#pragma once

#include <Alpha/Core/CoreMinimal.h>

#include <Alpha/Engine/Renderer/StaticMesh.h>


namespace Alpha
{
    class StaticMeshModel
    {
    public:
        /// Load a static mesh model from a file.
        bool Load(const std::string& filename);

        /// Load a static mesh model.
        /// @note: Use this constructor instead of using a static mesh.
        bool Load(const std::vector<Vertex>& vertices, const std::vector<uint32>& indices);

        /// Render the model.
        void Draw(const Pointer<Shader>& shader, TransformMatrix& transform, EDrawMode drawMode);

        /// Getter: The mesh's material.
        Pointer<Material> GetMaterial(uint32 index);

        /// Setter: The mesh's material.
        void SetMaterial(uint32 index, const Pointer<Material>& mat);

        /// Getter: The meshes of this model.
        inline std::vector<Pointer<StaticMesh>>& GetMeshes() { return m_meshes; }

    private:
        /// The meshes of this model.
        std::vector<Pointer<StaticMesh>> m_meshes;
    };
}
