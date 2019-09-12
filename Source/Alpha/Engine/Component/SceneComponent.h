#pragma once

#include <Alpha/Core/CoreMinimal.h>

namespace Alpha
{
    class SceneComponent
    {
    public:
        inline explicit SceneComponent(const Transform& transform) : m_transform(transform) {}

        virtual ~SceneComponent() = default;

        /// @getter: Position of the actor in the world.
        inline const Vector3& GetWorldLocation() const { return m_transform.location; }

        /// @setter: Position of the actor in the world.
        inline const Vector3& SetWorldLocation(const Vector3& location) { return m_transform.location = location; }

        /// @getter: The entity rotation (Euler angle).
        inline const Vector3& GetWorldRotation() const { return m_transform.rotation; }

        /// @setter: The entity rotation (Euler angle).
        inline const Vector3& SetWorldRotation(const Vector3 &rotation) { return m_transform.rotation = rotation; }

        /// @getter: The scale of this object.
        inline const Vector3& GetWorldScale() const { return m_transform.scale; }

        /// @setter: The scale of this object.
        inline const Vector3& SetWorldScale(const Vector3 &scale) { return m_transform.scale = scale; }

        /// @getter: The world transform of this object.
        inline Transform GetWorldTransform() { return m_transform; }

        /// @getter: The forward vector.
        inline Vector3 GetForwardVector() const { return CalculateForwardVector(m_transform); }

        /// @getter: The right vector.
        inline Vector3 GetRightVector() const { return CalculateRightVector(m_transform); }

    private:
        Transform m_transform;
    };
}