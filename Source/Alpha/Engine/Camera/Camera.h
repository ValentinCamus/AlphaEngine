#pragma once

#include <Alpha/Core/CoreMinimal.h>

#include <Alpha/Engine/Component/SceneComponent.h>

namespace Alpha
{
    constexpr float DEFAULT_DELTA_TIME = 0.1f;

    // From LearnOpenGL tutorial : default camera values
    constexpr float YAW        = -90.0f;
    constexpr float PITCH      =  0.0f;
    constexpr float SPEED      =  10.0f;
    constexpr float SENSITIVTY =  0.25f;
    constexpr float ZOOM       =  45.0f;

    class Camera : public SceneComponent
    {
    public:
        explicit Camera(const Vector& location = Vector(0.f, 0.f, 1.f),
                        const Vector& rotation = Vector(0.f, 1.f, 0.f),
                        float zoom = 45.f)
                : SceneComponent(Transform(location, rotation))
                , m_zoom(zoom) {}

        ~Camera() override = default;

        /// Calculate the view matrix.
        inline Matrix4 GetViewMatrix() const { return MakeViewMatrix(GetWorldLocation(), GetWorldRotation()); }

    public:
        /// @getter : Camera zoom.
        inline float  GetZoom() const { return m_zoom; }

        /// @setter : Camera zoom.
        inline float SetZoom(float zoom) { return m_zoom = zoom; }

    private:
        /// Camera zoom.
        float m_zoom;
    };

}