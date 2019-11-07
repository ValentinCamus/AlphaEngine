#include "StaticMeshModel.h"

#include <Alpha/Engine/Loader/MeshLoader.h>

namespace Alpha
{
    Pointer<StaticMeshModel> StaticMeshModel::Create(const std::string &filename)
    {
        Pointer<StaticMeshModel> model = NewPointer<StaticMeshModel>();
        model->Load(filename);
        return model;
    }

    Pointer<StaticMeshModel> StaticMeshModel::Create(const Vertices& vertices, const Indices& indices)
    {
        Pointer<StaticMeshModel> model = NewPointer<StaticMeshModel>();
        model->Load(vertices, indices);
        return model;
    }

    bool StaticMeshModel::Load(const std::string &filename)
    {
        bool bIsValid = MeshLoader::Load(filename);

        if (bIsValid)
        {
            m_meshes = MeshLoader::Get();
            MeshLoader::Flush();
        }

        return bIsValid;
    }

    bool StaticMeshModel::Load(const std::vector<Vertex> &vertices, const std::vector<uint32> &indices)
    {
        Pointer<StaticMesh> sm = StaticMesh::Create(vertices, indices);
        m_meshes = {sm};
        return true;
    }

    Pointer<Material> StaticMeshModel::GetMaterial(uint32 index)
    {
        return m_meshes.at(index)->GetMaterial();
    }

    void StaticMeshModel::SetMaterial(uint32 index, const Pointer<Material> &mat)
    {
        return m_meshes.at(index)->SetMaterial(mat);
    }

    void StaticMeshModel::Draw(const Pointer<Shader>& shader,
                               const Matrix4x4 * projection,
                               const Matrix4x4 * view,
                               const Matrix4x4 * model) const
    {
        for (const Pointer<StaticMesh>& sm : m_meshes) sm->Draw(shader, projection, view, model);
    }
}