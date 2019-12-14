#include "SkeletalMeshModel.h"

#include <Alpha/Engine/Loader/StaticMeshLoader.h>

namespace Alpha
{
    Pointer<SkeletalMeshModel> SkeletalMeshModel::Create(const std::string &filename)
    {
        Pointer<SkeletalMeshModel> model = NewPointer<SkeletalMeshModel>();
        model->Load(filename);
        return model;
    }

    Pointer<SkeletalMeshModel> SkeletalMeshModel::Create(const Pointer<Skeleton> skeleton,
                                                         const Vertices& vertices,
                                                         const Indices& indices)
    {
        m_skeleton = skeleton;
        Pointer<SkeletalMeshModel> model = NewPointer<SkeletalMeshModel>();
        model->Load(vertices, indices);
        return model;
    }

    bool SkeletalMeshModel::Load(const std::string &filename)
    {
//        bool bIsValid = MeshLoader::Load(filename);
//
//        if (bIsValid)
//        {
//            m_meshes = MeshLoader::Get();
//            MeshLoader::Flush();
//        }
//
//        return bIsValid;

        ALPHA_ASSERT("SkeletalMeshModel::Load not implemented");
        return false;
    }

    bool SkeletalMeshModel::Load(const std::vector<SkeletalMesh::Vertex> &vertices, const std::vector<uint32> &indices)
    {
        Pointer<SkeletalMesh> sm = SkeletalMesh::Create(vertices, indices);
        m_meshes = {sm};
        return true;
    }

    Pointer<Material> SkeletalMeshModel::GetMaterial(uint32 index)
    {
        return m_meshes.at(index)->GetMaterial();
    }

    void SkeletalMeshModel::SetMaterial(uint32 index, const Pointer<Material> &mat)
    {
        return m_meshes.at(index)->SetMaterial(mat);
    }

    void SkeletalMeshModel::Draw(const Pointer<Shader>& shader,
                               const Matrix4x4 * projection,
                               const Matrix4x4 * view,
                               const Matrix4x4 * model) const
    {
        for (const Pointer<SkeletalMesh>& sm : m_meshes) sm->Draw(shader, projection, view, model);
    }
}