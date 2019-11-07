#pragma once

#include <Alpha/Core/CoreMinimal.h>

#include <Alpha/Engine/Renderer/StaticMesh.h>


namespace Alpha
{
    class StaticMeshModel
    {
    public:
        using Vertices = std::vector<Vertex>;
        using Indices = std::vector<uint32>;

        static Pointer<StaticMeshModel> Create(const std::string& filename);
        static Pointer<StaticMeshModel> Create(const Vertices& vertices, const Indices& indices);

    public:
        /// Load a static mesh model from a file.
        bool Load(const std::string& filename);

        /// Load a static mesh model.
        /// @note: Use this constructor instead of using a static mesh.
        bool Load(const std::vector<Vertex>& vertices, const std::vector<uint32>& indices);

        /// Draw the model.
        void Draw(const Pointer<Shader>& shader,
                  const Matrix4x4 * projection = nullptr,
                  const Matrix4x4 * view = nullptr,
                  const Matrix4x4 * model = nullptr) const;

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
