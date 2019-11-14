#pragma once

#include <Alpha/Core/CoreMinimal.h>
#include <Alpha/Engine/Component/SceneComponent.h>

#include <Alpha/Engine/Renderer/DepthBuffer.h>

namespace Alpha
{
    class Light : public SceneComponent
    {
    public:
        struct Attenuation
        {
            float constant = 1.0f;
            float linear = 0.09;
            float quadractic = 0.032;
        };

        enum LightType
        {
            Directional = 0,
            Point = 1,
            Spot = 2
        };

    public:
        explicit Light(const LightType& type   = LightType::Directional,
                       const Vector& location  = Vector(0.0f),
                       const Vector& direction = Vector(0.0f),
                       const Color4& color     = Color4(1.0f))
                : SceneComponent(Transform(location, direction))
                , m_color(color)
                , m_type(type) {}

        ~Light() override = default;

    public:
        inline const LightType& GetType() const { return m_type; };

        inline const Color4& GetColor() const { return m_color; }
        inline const Color4& SetColor(const Color4& color) { return m_color = color; }

        inline const Pointer<DepthBuffer> GetDepthBuffer() const { return m_depthBuffer; }
        inline void SetDepthBuffer(const Pointer<DepthBuffer>& depthBuffer) { m_depthBuffer = depthBuffer; }
        inline bool IsDepthBufferValid() const { return m_depthBuffer != nullptr; }

        inline const Matrix4x4& GetViewProjectionMatrix() const { return m_viewProjectionMatrix; }
        inline void SetViewProjectionMatrix(const Matrix4x4 &matrix) { m_viewProjectionMatrix = matrix; }

        inline bool IsShadowingEnable() const { return IsDepthBufferValid(); }

        inline void CreateShadowMap(uint32 width, uint32 height)
        {
            m_depthBuffer = DepthBuffer::Create(width, height);
            ALPHA_ASSERT(IsDepthBufferValid(), "Failed to initialize Shadow Map");
        }

        virtual const Matrix4x4& CalculateViewProjectionMatrix() = 0;

    private:
        /// The light color/tint.
        Color4 m_color;

        /// The light type: {Directional, Spot, Point}
        LightType m_type;

        /// A transformation to transform a 3D point in the light space.
        Matrix4x4 m_viewProjectionMatrix = Matrix4x4(1.0f);

        /// The container of the shadow map.
        Pointer<DepthBuffer> m_depthBuffer = nullptr;
    };
}