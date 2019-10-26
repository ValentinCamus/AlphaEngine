#include "EulerCamera.h"

namespace Alpha
{
    EulerCamera::EulerCamera(const Vector& location, const Vector& rotation)
            : Camera(location, rotation, ZOOM)
            , m_movementSpeed(SPEED)
            , m_mouseSensitivity(SENSITIVITY) {}

    void EulerCamera::MoveForward(float axisValue)
    {
        float velocity = m_movementSpeed * m_deltaTime;
        Vector newPosition = GetWorldLocation() + GetForwardVector() * axisValue * velocity;
        SetWorldLocation(newPosition);
    }

    void EulerCamera::MoveRight(float axisValue)
    {
        float velocity = m_movementSpeed * m_deltaTime;
        Vector newPosition = GetWorldLocation() + GetRightVector() * axisValue * velocity;
        SetWorldLocation(newPosition);
    }

    void EulerCamera::Look(const Vector2& mousePosition)
    {
        float xPos = mousePosition.x;
        float yPos = mousePosition.y;
        float xOffset = xPos - m_mouseX;
        float yOffset = m_mouseY - yPos;

        m_mouseX = xPos;
        m_mouseY = yPos;

        // Do only the camera movement if xOffset or yOffset are small.
        // This check avoid camera sudden movement.
        if (std::abs(xOffset) < 20.f && std::abs(yOffset) < 20.f)
        {
            xOffset *= m_mouseSensitivity;
            yOffset *= m_mouseSensitivity;

            Vector newRotation = GetWorldRotation() + Vector(-yOffset, xOffset, 0.0f);

            // Make sure that when pitch is out of bounds, screen doesn't get flipped.
            if (m_isPitchConstrained)
            {
                if (newRotation.x >  89.0f) newRotation.x =  89.0f;
                if (newRotation.x < -89.0f) newRotation.x = -89.0f;
            }

            SetWorldRotation(newRotation);
        }
    }
}