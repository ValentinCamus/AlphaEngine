#pragma once

#include <Alpha/Core/CoreMinimal.h>

#include <Alpha/Engine/Renderer/Shader.h>
#include <Alpha/Engine/Material/Material.h>
#include <Alpha/Engine/Component/SceneComponent.h>
#include <Alpha/Engine/StaticMeshModel/StaticMeshModel.h>

namespace Alpha
{
    class StaticMeshEntity : public SceneComponent
    {
    public:
        /// Constructor.
        explicit StaticMeshEntity(std::string name, const Pointer<StaticMeshModel>& model);

        /// Draw the entity.
        void Draw(const Pointer<Shader>& shader, TransformMatrix& transform);

    public:
        /// The number of material.
        inline uint32 GetNbMaterial() const { return m_materials.size(); }

        /// @getter: The entity's material.
        inline const Pointer<Material>& GetMaterial(uint32 index) { return m_materials[index]; }

        /// @setter: The entity's material.
        inline void SetMaterial(uint32 index, const Pointer<Material>& mat) { m_materials[index] = mat; }

        /// @getter: The entity name.
        inline const std::string& GetName() const { return m_name; }

        /// @getter: The model matrix.
        /// @warning: Perform a calculation.
        inline Matrix4x4 GetModelMatrix() const
        {
            Vector location = GetWorldLocation();
            Vector rotation = GetWorldRotation();
            Vector scale = GetWorldScale();
            return MakeModelMatrix(location, rotation, scale);
        }

    private:
        /// The entity name.
        std::string m_name;

        /// The model of the entity.
        Pointer<StaticMeshModel> m_model;

        /// The list of material to apply to the entity when it will be drawn.
        std::vector<Pointer<Material>> m_materials;
    };
}