#pragma once


#include "Camera.h"

#include <Alpha/Core/CoreMinimal.h>

namespace Alpha
{
    class EulerCamera : public Camera

    {
    public:
        explicit EulerCamera(const Vector& location = Vector(0), const Vector& rotation = Vector(0.0f, 1.0f, 0.0f));

        ~EulerCamera() override = default;

        void MoveForward(float axisValue = 1.0f);
        void MoveRight(float axisValue = 1.0f);
        void Look(const Vector2& mousePosition);

    public:
        /// @getter : Is pitch constrained ?
        inline bool IsPitchConstrained() const { return m_isPitchConstrained; }

        /// @setter : Is pitch constrained ?
        inline bool SetPitchConstrained(bool constraint) { return (m_isPitchConstrained = constraint); }

    private:
        /// Is pitch constrained ?
        bool m_isPitchConstrained = true;

        float m_deltaTime = 0.01f;

        /// Camera options: Speed.
        float m_movementSpeed;

        /// Camera options: Sensitivity.
        float m_mouseSensitivity;

        /// Last mouse position on X-Axis.
        float m_mouseX = 0.0f;

        /// Last mouse position on Y-Axis.
        float m_mouseY = 0.0f;
    };
}