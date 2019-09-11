#pragma once

#include <Alpha/Core/CoreMinimal.h>
#include <Alpha/Engine/Light/Light.h>

namespace Alpha
{
    class PointLight : public Light
    {
    public:
        explicit PointLight(const Vector& location = Vector(0.0f),
                            const Color4& color = Color4(1.0f))
            : Light(LightType::Point, location, Vector(0.0f), color)
        {

        }

        ~PointLight() override = default;

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

    public:
        Attenuation m_attenuation;
    };
}