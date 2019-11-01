#include "StaticMeshEntity.h"

namespace Alpha
{
    StaticMeshEntity::StaticMeshEntity(std::string name, const Pointer<StaticMeshModel> &model)
            : SceneComponent(Transform(), name)
            , m_name(std::move(name))
            , m_model(model)
    {
        auto nMeshes = model->GetMeshes().size();
        m_materials = std::vector<Pointer<Material>>(nMeshes);
    }

    void StaticMeshEntity::Draw(const Pointer<Shader> &shader, TransformMatrix &transform)
    {
        transform.model = GetModelMatrix();

        if (Renderer::IsDisable(Renderer::EOption::DiscardMaterial))
        {
            for (uint32 i = 0; i < m_materials.size(); ++i)
            {
                Pointer<Material> mat = m_materials[i];
                m_model->SetMaterial(i, mat);
            }
        }

        m_model->Draw(shader, transform, m_drawMode);
    }

    Matrix4x4 StaticMeshEntity::GetModelMatrix() const
    {
        return MakeModelMatrix(GetWorldLocation(), GetWorldRotation(), GetWorldScale());
    }

    const Pointer<Material> &StaticMeshEntity::GetMaterial(uint32 index)
    {
        ALPHA_ASSERT(index < GetNbMaterial(), "{0}: Material {1} doesn't exist", m_name, index);
        return m_materials.at(index);
    }

    void StaticMeshEntity::SetMaterial(uint32 index, const Pointer<Material> &mat)
    {
        ALPHA_ASSERT(index < GetNbMaterial(), "{0}: Material {1} doesn't exist", m_name, index);
        m_materials.at(index) = mat;
    }
}