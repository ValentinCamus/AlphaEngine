#pragma once

#include <Alpha/Core/CoreMinimal.h>

namespace Alpha
{
    class BoneTransform
    {
    public:
        inline BoneTransform(Vector3 position, Quaternion rotation)
            : m_position(position)
            , m_rotation(rotation)
        {

        }

        inline Matrix4x4 GetLocalTransform() const
        {
            Matrix4x4 transform = Matrix4x4(1.0f);
            transform = glm::translate(transform, m_position);
            transform = glm::toMat4(m_rotation) * transform;
            return transform;
        }

    public:
        static inline BoneTransform Lerp(const BoneTransform& a, const BoneTransform& b, float t)
        {
           Vector3 pos = glm::lerp(a.m_position, b.m_position, t);
           Quaternion rot = glm::lerp(a.m_rotation, b.m_rotation, t);
           return { pos, rot };
        }

    private:
        Vector3 m_position; // The position relative to the parent bone
        Quaternion m_rotation; // The rotation relative to the parent bone
    };
}
