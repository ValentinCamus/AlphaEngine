#pragma once

#include <Alpha/Core/CoreMinimal.h>
#include <Alpha/Engine/Light/Light.h>

#include <Alpha/Engine/Camera/Camera.h>

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

    public:
        Light::Attenuation m_attenuation;
    };
}