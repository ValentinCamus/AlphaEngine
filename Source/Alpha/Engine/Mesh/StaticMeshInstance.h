#pragma once

#include <Alpha/Core/CoreMinimal.h>

#include <Alpha/Engine/Renderer/Shader.h>
#include <Alpha/Engine/Material/Material.h>
#include <Alpha/Engine/Component/SceneComponent.h>
#include <Alpha/Engine/Mesh/StaticMeshModel.h>

namespace Alpha
{
    class StaticMeshInstance : public SceneComponent
    {
    public:
        /// Constructor.
        explicit StaticMeshInstance(std::string name, const Pointer<StaticMeshModel>& model);

        /// Draw the entity.
        /// The model matrix will be set with the instance's transform.
        void Draw(const Pointer<Shader>& shader,
                  const Matrix4x4 * projection = nullptr,
                  const Matrix4x4 * view = nullptr) const;

        /// Draw the entity.
        void Draw(const Pointer<Shader>& shader,
                  const Matrix4x4 * projection,
                  const Matrix4x4 * view,
                  const Matrix4x4 * model) const;

    public:
        /// The number of material.
        inline uint32 GetNbMaterial() const { return static_cast<uint32>(m_materials.size()); }

        /// @getter: The entity's material.
        const Pointer<Material>& GetMaterial(uint32 index);

        /// @setter: The entity's material.
        void SetMaterial(uint32 index, const Pointer<Material>& mat);

        /// @getter: The entity name.
        inline const std::string& GetName() const override { return m_name; }

        /// @getter: The model matrix.
        /// @warning: Perform a calculation.
        Matrix4x4 GetModelMatrix() const;

        inline Renderer::EDrawMode GetDrawMode() const { return m_drawMode; }

        inline void SetDrawMode(Renderer::EDrawMode drawMode) { m_drawMode = drawMode; }

        inline void BindMaterials() const { Renderer::GetDrawOptions()->bUseMaterial = true; }

        inline void UnbindMaterials() const { Renderer::GetDrawOptions()->bUseMaterial = false; }

    private:
        void SetModelMaterials() const;

    private:
        /// The entity name.
        std::string m_name;

        /// The model of the entity.
        Pointer<StaticMeshModel> m_model;

        /// The list of material to apply to the entity when it will be drawn.
        std::vector<Pointer<Material>> m_materials;

        Renderer::EDrawMode m_drawMode = Renderer::EDrawMode::Triangles;
    };
}