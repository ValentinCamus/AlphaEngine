#include "StaticMeshModel.h"

#include <Alpha/Engine/Loader/MeshLoader.h>

namespace Alpha
{
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

    void StaticMeshModel::Draw(const Pointer<Shader>& shader, TransformMatrix& transform)
    {
        for (Pointer<StaticMesh>& sm : GetMeshes()) sm->Draw(shader, transform);
    }
}