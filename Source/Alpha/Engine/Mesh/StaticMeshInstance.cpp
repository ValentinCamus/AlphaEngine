#include "StaticMeshInstance.h"

namespace Alpha
{
    StaticMeshInstance::StaticMeshInstance(std::string name, const Pointer<StaticMeshModel> &model)
            : SceneComponent(Transform(), name)
            , m_name(std::move(name))
            , m_model(model)
    {
        auto nMeshes = model->GetMeshes().size();
        m_materials = std::vector<Pointer<Material>>(nMeshes);
    }

    void StaticMeshInstance::Draw(const Pointer<Shader> &shader, TransformMatrix &transform)
    {
        Renderer::GetDrawOptions()->drawMode = m_drawMode;

        transform.model = GetModelMatrix();

        if (Renderer::GetDrawOptions()->bUseMaterial)
        {
            for (uint32 i = 0; i < m_materials.size(); ++i)
            {
                Pointer<Material> mat = m_materials[i];
                m_model->SetMaterial(i, mat);
            }
        }

        m_model->Draw(shader, transform);
    }

    Matrix4x4 StaticMeshInstance::GetModelMatrix() const
    {
        return MakeModelMatrix(GetWorldLocation(), GetWorldRotation(), GetWorldScale());
    }

    const Pointer<Material> &StaticMeshInstance::GetMaterial(uint32 index)
    {
        ALPHA_ASSERT(index < GetNbMaterial(), "{0}: Material {1} doesn't exist", m_name, index);
        return m_materials.at(index);
    }

    void StaticMeshInstance::SetMaterial(uint32 index, const Pointer<Material> &mat)
    {
        ALPHA_ASSERT(index < GetNbMaterial(), "{0}: Material {1} doesn't exist", m_name, index);
        m_materials.at(index) = mat;
    }
}