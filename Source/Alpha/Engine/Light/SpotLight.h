#pragma once

#include <Alpha/Core/CoreMinimal.h>
#include <Alpha/Engine/Light/Light.h>

#include <Alpha/Engine/Camera/Camera.h>

namespace Alpha
{
    class SpotLight : public Light
    {
    public:
        explicit SpotLight(float cutOff,
                           const Vector& location  = Vector(0.0f),
                           const Vector& direction = Vector(0.0f),
                           const Color4& color     = Color4(1.0f))
               : Light(LightType::Spot, location, direction, color)
               , m_cutOff(cutOff)
        {

        }

        ~SpotLight() override = default;

    public:
        inline float SetCutOffInRadians(float angle) { return m_cutOff = angle; }
        inline float SetCutOffInDegrees(float angle) { return m_cutOff = angle * DEG2RAD; }

        inline float GetCutOff() const { return m_cutOff; }

        inline void SetAttenuation(const Light::Attenuation& attenuation) { m_attenuation = attenuation; }
        inline const Light::Attenuation& GetAttenuation() const { return m_attenuation; }

        inline const Matrix4x4& CalculateViewProjectionMatrix() override
        {
            ALPHA_ASSERT(IsDepthBufferValid(), "Invalid Shadow Map");

            Camera camera = Camera(GetWorldLocation(), GetWorldRotation());
            camera.SetViewType(Camera::EViewType::VT_Perspective);

            float aspectRatio = GetDepthBuffer()->GetAspectRatio();
            Matrix4x4 lightProjection = glm::perspective(camera.GetZoom(), aspectRatio, 1.0f, 7.5f);
            Matrix4x4 lightView = MakeViewMatrix(camera.GetWorldLocation(), camera.GetWorldRotation());
            Matrix4x4 lightSpace = lightProjection * lightView;

            SetViewProjectionMatrix(lightSpace);

            return GetViewProjectionMatrix();
        }

    private:
        /// The inner angle.
        float m_cutOff;

        /// The attenuation.
        Light::Attenuation m_attenuation;
    };
}