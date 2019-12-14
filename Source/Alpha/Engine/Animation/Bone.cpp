#include "Bone.h"

namespace Alpha
{
    void Bone::CalculateInverseOriginalTransform(const Matrix4x4 &parentOriginalTransform)
    {
        Matrix4x4 originalTransform = parentOriginalTransform * m_localOriginalTransform;
        m_inverseOriginalTransform = glm::inverse(originalTransform);

        for (const Pointer<Bone>& child : m_children)
        {
            child->CalculateInverseOriginalTransform(originalTransform);
        }
    }
}