#pragma once

#include <Alpha/Core/CoreMinimal.h>

namespace Alpha
{
	static uint32 nComponents = 0;

    class SceneComponent
    {
    public:
		inline explicit SceneComponent(const Transform& transform)
			: m_transform(transform)
		{
			m_name = "NoName_" + ToString(nComponents++);
		}

		inline explicit SceneComponent(const Transform& transform, const std::string& name)
			: m_transform(transform)
			, m_name(name)
		{
			++nComponents;
		}

        virtual ~SceneComponent() = default;

        /// @getter: Position of the actor in the world.
        inline Vector3 GetWorldLocation() const { return m_transform.location; }

        /// @setter: Position of the actor in the world.
        inline void SetWorldLocation(const Vector3& location) { m_transform.location = location; }

        /// @getter: The entity rotation (Euler angle).
        inline Vector3 GetWorldRotation() const { return m_transform.rotation; }

        /// @setter: The entity rotation (Euler angle).
        inline void SetWorldRotation(const Vector3 &rotation) { m_transform.rotation = rotation; }

        /// @getter: The scale of this object.
        inline Vector3 GetWorldScale() const { return m_transform.scale; }

        /// @setter: The scale of this object.
        inline void SetWorldScale(const Vector3 &scale) { m_transform.scale = scale; }

        /// @getter: The world transform of this object.
        inline const Transform& GetWorldTransform() { return m_transform; }

        /// @getter: The forward vector.
        inline Vector3 GetForwardVector() const { return CalculateForwardVector(m_transform); }

        /// @getter: The right vector.
        inline Vector3 GetRightVector() const { return CalculateRightVector(m_transform); }

        virtual inline const std::string& GetName() const { return m_name; }

		inline void SetName(const std::string& name) { m_name = name; }

    private:
		Transform m_transform;

		std::string m_name;
    };
}