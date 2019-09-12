#pragma once

#include <Alpha/Core/CoreMinimal.h>

namespace Alpha
{
    class Input
    {
    protected:
        Input() = default;

    public:
        Input(const Input &) = delete;

        virtual ~Input() = default;

        Input& operator=(const Input &) = delete;

        inline static bool IsKeyPressed(int keyCode)
        {
            return s_instance->IsKeyPressedImpl(keyCode);
        }

        inline static bool IsMouseButtonPressed(int button)
        {
            return s_instance->IsMouseButtonPressedImpl(button);
        }

        inline static Vector2 GetMousePosition()
        {
            return s_instance->GetMousePositionImpl();
        }

        inline static float GetMouseX()
        {
            return s_instance->GetMouseXImpl();
        }

        inline static float GetMouseY()
        {
            return s_instance->GetMouseYImpl();
        }

    protected:
        virtual bool IsKeyPressedImpl(int keyCode) = 0;

        virtual bool IsMouseButtonPressedImpl(int button) = 0;

        virtual Vector2 GetMousePositionImpl() = 0;

        virtual float GetMouseXImpl() = 0;

        virtual float GetMouseYImpl() = 0;

    private:
        static Input * s_instance;
    };
};