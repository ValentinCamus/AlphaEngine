#pragma once

#include <Alpha/Core/CoreMinimal.h>
#include <Alpha/Engine/Light/Light.h>

#include <Alpha/Engine/Camera/Camera.h>

namespace Alpha
{
    class DirectionalLight : public Light
    {
    public:
        /// Constructor.
        explicit DirectionalLight(const Vector3& direction = Vector3(0.0f, 0.0f, 0.0f),
                                  const Color4& color = Color4(1.0f))
            : Light(Directional, Vector(0.0f, 0.0f, 0.0f), direction, color)
        {

        }

        /// Destructor.
        ~DirectionalLight() override = default;

        inline const Matrix4x4& CalculateViewProjectionMatrix() override
        {
            ALPHA_ASSERT(IsDepthBufferValid(), "Invalid Shadow Map");

            Camera camera = Camera(GetWorldLocation(), GetWorldRotation());
            camera.SetViewType(Camera::EViewType::VT_Orthographic);

            Matrix4x4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);
            Matrix4x4 lightView = MakeViewMatrix(GetWorldLocation(), GetWorldRotation());
            Matrix4x4 lightSpace = lightProjection * lightView;

            SetViewProjectionMatrix(lightSpace);

            return GetViewProjectionMatrix();
        }

    };
}