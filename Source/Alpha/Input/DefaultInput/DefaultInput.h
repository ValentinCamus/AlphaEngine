#pragma once

#include <Alpha/Core/CoreMinimal.h>
#include <Alpha/Input/Input.h>

namespace Alpha
{
    class DefaultInput : public Input
    {
    public:
        explicit DefaultInput();

    protected:
        bool IsKeyPressedImpl(int32 keyCode) override;

        bool IsMouseButtonPressedImpl(int32 button) override;

        Vector2 GetMousePositionImpl() override;

        float GetMouseXImpl() override;

        float GetMouseYImpl() override;
    };
}


