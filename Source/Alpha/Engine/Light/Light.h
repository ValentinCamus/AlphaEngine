#pragma once

#include <Alpha/Core/CoreMinimal.h>
#include <Alpha/Engine/Component/SceneComponent.h>

#include <Alpha/Engine/Renderer/DepthBuffer.h>

namespace Alpha
{
    struct Attenuation
    {
        float constant;
        float linear;
        float quadratic;

        Attenuation() : constant(1.0), linear(), quadratic() {}
    };

    class Light : public SceneComponent
    {
    public:
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
                : SceneComponent(Transform(location))
                , m_color(color)
                , m_type(type)
                , m_direction(direction)
        {

        }

        ~Light() override = default;

    public:
        inline const LightType& GetType() const { return m_type; };

        inline const Color4& GetColor() const { return m_color; }
        inline const Color4& SetColor(const Color4& color) { return m_color = color; }

        inline const Vector& GetDirection() const { return m_direction; }
        inline const Vector& SetDirection(const Vector& dir) { return m_direction = dir; }

        inline const Pointer<DepthBuffer> GetDepthBuffer() const { return m_depthBuffer; }
        inline void SetDepthBuffer(const Pointer<DepthBuffer> &depthBuffer) { m_depthBuffer = depthBuffer; }
        inline bool IsDepthBufferValid() const { return m_depthBuffer != nullptr; }

        inline const Matrix4x4& GetSpace() const { return m_space; }
        inline void SetSpace(const Matrix4x4& space) { m_space = space; }

    private:
        Color4 m_color;

        LightType m_type;

        Vector m_direction;

        Matrix4x4 m_space;

        Pointer<DepthBuffer> m_depthBuffer = nullptr;
    };
}