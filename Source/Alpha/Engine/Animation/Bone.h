#include <utility>

#pragma once

#include <Alpha/Core/CoreMinimal.h>

namespace Alpha
{
    class Bone
    {
    public:
        inline Bone(uint32 index, std::string name, const Matrix4x4& localOriginalTransform)
            : m_index(index)
            , m_name(std::move(name))
            , m_localOriginalTransform(localOriginalTransform) {}


        /// Adds a child bone to this bone. Used during the creation of the bone hierarchy.
        /// Bones can have multiple children, which is why they are stored in a vector
        /// (e.g. a "hand" bone may have multiple "finger" children bones).
        inline void AddChild(const Pointer<Bone>& child) { m_children.push_back(child); }

        void CalculateInverseOriginalTransform(const Matrix4x4& parentOriginalTransform);

    public:
        inline uint32 GetIndex() const { return m_index; }

        inline const Matrix4x4& GetAnimationTransform() const { return m_animationTransform; }
        inline void SetAnimationTransform(const Matrix4x4 &transform) { m_animationTransform = transform; }

        inline const std::string& GetName() const { return m_name; }
        inline void SetName(const std::string& name) { m_name = name; }

        inline const std::vector<Pointer<Bone>>& GetChildren() const { return m_children; }

        inline const Matrix4x4& GetInverseOriginalTransform() const { return m_inverseOriginalTransform; }

    private:
        uint32 m_index;

        /// The name of the bone. This is how the bone is named in the
        /// file, and so is used to identify which bone a bone
        /// transform in an animation keyframe refers to.
        std::string m_name;

        std::vector<Pointer<Bone>> m_children;

        /// [model space]
        Matrix4x4 m_animationTransform = Matrix4x4(1.0f);

        /// [bone space]
        /// The bone-space transform of the bone in the bind position.
        Matrix4x4 m_localOriginalTransform = Matrix4x4(1.0f);

        /// [model space]
        Matrix4x4 m_inverseOriginalTransform = Matrix4x4(1.0f);
    };
}
