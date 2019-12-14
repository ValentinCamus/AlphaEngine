#pragma once

#include <Alpha/Core/CoreMinimal.h>

#include <Alpha/Engine/Renderer/SkeletalMesh.h>

#include <Alpha/Engine/Animation/Bone.h>

namespace Alpha
{
    class SkeletalMeshModel
    {
    public:
        struct Skeleton
        {
            uint32 nBones;
            Pointer<Bone> root;

            inline void Init()
            {
                Matrix4x4 transform = Matrix4x4(1.0f);
                rootBone->CalculateInverseOriginalTransform(transform);
            }
        };

        using Vertices = std::vector<SkeletalMesh::Vertex>;
        using Indices = std::vector<uint32>;

        static Pointer<SkeletalMeshModel> Create(const std::string& filename);

        static Pointer<SkeletalMeshModel> Create(const Pointer<Skeleton> skeleton,
                                                 const Vertices& vertices,
                                                 const Indices& indices);

    public:
        /// Load a static mesh model from a file.
        bool Load(const std::string& filename);

        /// Load a static mesh model.
        /// @note: Use this constructor instead of using a static mesh.
        bool Load(const std::vector<SkeletalMesh::Vertex>& vertices, const std::vector<uint32>& indices);

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
        inline std::vector<Pointer<SkeletalMesh>>& GetMeshes() { return m_meshes; }

        inline const Pointer<Skeleton>& GetSkeleton() const { return m_skeleton; }
        inline void SetSkeleton(const Pointer<Skeleton>& skeleton) { m_skeleton = skeleton; }

        /// Gets an array of the all important model-space transforms of all the
        /// bones (with the current animation pose applied) in the entity. The
        /// bones are ordered in the array based on their bone index. The position
        /// of each bone's transform in the array is equal to the bone's index.
        inline std::vector<Matrix4x4> GetBoneTransforms()
        {
            std::vector<Matrix4x4> bonesMatrices(m_skeleton->nBones);
            AddBonesTransformRecursively(m_skeleton->root)
        }

        inline void AddBonesTransformRecursively(Pointer<Bone> bone, std::vector<Matrix4x4> &matrices)
        {
            matrices[bone->GetIndex()] = bone->GetAnimationTransform();

            for (const Pointer<Bone>& child : bone->GetChildren())
            {
                AddBonesTransformRecursively(child, matrices);
            }
        }

    private:
        /// The meshes of this model.
        std::vector<Pointer<SkeletalMesh>> m_meshes;

        /// The skeleton of this model.
        Pointer<Skeleton> m_skeleton;

        // Pointer<Animator> m_animator;
    };
}
