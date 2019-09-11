#pragma once

#include <Alpha/Core/CoreMinimal.h>
#include <Alpha/Engine/Light/Light.h>

namespace Alpha
{
    class SpotLight : public Light
    {
    public:
        explicit SpotLight(float innerAngle,
                           float outerAngle,
                           const Vector& location  = Vector(0.0f),
                           const Vector& direction = Vector(0.0f),
                           const Color4& color     = Color4(1.0f ))
               : Light(LightType::Spot, location, direction, color)
               , m_innerAngle(innerAngle)
               , m_outerAngle(outerAngle)
        {

        }

        ~SpotLight() override = default;

    public:
        inline float SetInnerAngleInRadians(float angle) { return m_innerAngle = angle; }
        inline float SetOuterAngleInRadians(float angle) { return m_outerAngle = angle; }

        inline float SetInnerAngleInDegrees(float angle) { return m_innerAngle = angle * TO_RAD; }
        inline float SetOuterAngleInDegrees(float angle) { return m_outerAngle = angle * TO_RAD; }

        inline float GetInnerAngle() const { return m_innerAngle; }
        inline float GetOuterAngle() const { return m_outerAngle; }

        inline const Attenuation& SetAttenuation(const Attenuation& attenuation)
        {
            return m_attenuation = attenuation;
        }

        inline const Attenuation& SetAttenuation(float constant, float linear, float quadratic)
        {
            m_attenuation.constant = constant;
            m_attenuation.linear = linear;
            m_attenuation.quadratic = quadratic;
            return m_attenuation;
        }

        inline const Attenuation& GetAttenuation() const { return m_attenuation; }

    private:
        /// The inner angle.
        float m_innerAngle;

        /// The outer angle.
        float m_outerAngle;

        /// The attenuation.
        Attenuation m_attenuation;
    };
}