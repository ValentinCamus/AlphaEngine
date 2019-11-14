#pragma once

#include <Alpha/Core/CoreMinimal.h>

#include <Alpha/Engine/Component/SceneComponent.h>

namespace Alpha
{
    // Default camera values
    constexpr float DEFAULT_CAMERA_ZOOM =  45.0f;
    constexpr float DEFAULT_CAMERA_SPEED =  10.0f;
    constexpr float DEFAULT_CAMERA_SENSITIVITY =  0.25f;

    class Camera : public SceneComponent
    {
    public:
        enum class EViewType
        {
            VT_Orthographic,
            VT_Perspective,
        };

    public:
        explicit Camera(const Vector& location = Vector(0.f, 0.f, 1.f),
                        const Vector& rotation = Vector(0.f, 1.f, 0.f),
                        float zoom = 45.f)
                : SceneComponent(Transform(location, rotation))
                , m_zoom(zoom) {}

        ~Camera() override = default;

    public:
        /// @getter : Camera zoom.
        inline float  GetZoom() const { return m_zoom; }

        /// @setter : Camera zoom.
        inline float SetZoom(float zoom) { return m_zoom = zoom; }

        inline EViewType GetViewType() const { return  m_viewType; }

        inline void SetViewType(EViewType type) { m_viewType = type; }

        inline Matrix4x4 GetProjectionMatrix(float aspectRatio) const
        {
            switch (GetViewType())
            {
                case EViewType::VT_Perspective: return CalculatePerpectiveMatrix(aspectRatio);
                case EViewType::VT_Orthographic: return CalculateOrthographicProjection(aspectRatio);
                default: ALPHA_ASSERT(false, "Camera::GetProjectionMatrix: Undefined view type"); break;
            }
            return Matrix4x4(1.0f);
        }

    private:

        inline Matrix4x4 CalculateOrthographicProjection(float aspectRatio) const
        {
            float top = GetZoom();
            float bottom = -GetZoom();
            float left = -aspectRatio * GetZoom();
            float right = aspectRatio * GetZoom();
            return MakeOrthographicMatrix(left, right, bottom, top);
        }

        inline Matrix4x4 CalculatePerpectiveMatrix(float aspectRatio) const
        {
            return MakePerspectiveMatrix(GetZoom(), aspectRatio);
        }

    private:
        /// Camera zoom.
        float m_zoom;

        EViewType m_viewType = EViewType::VT_Perspective;
    };

    using StaticCamera = Camera;
}