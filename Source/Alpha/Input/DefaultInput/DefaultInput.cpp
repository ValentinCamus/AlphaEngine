#include "DefaultInput.h"

#include <Alpha/Application/Application.h>
#include <GLFW/glfw3.h>

namespace Alpha
{
    Input* Input::s_instance = new DefaultInput();

    DefaultInput::DefaultInput()
    {
        Logger::Info("Input system: Use DefaultInput");
        Logger::Warn("AlphaEngine supports only US keyboard binding (aka QWERTY)");
    }

    bool DefaultInput::IsKeyPressedImpl(int keyCode)
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetNativeWindow());
        auto state = glfwGetKey(window, keyCode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool DefaultInput::IsMouseButtonPressedImpl(int button)
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetNativeWindow());
        auto state = glfwGetMouseButton(window, button);
        return state == GLFW_PRESS;
    }

    Vector2 DefaultInput::GetMousePositionImpl()
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetNativeWindow());
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);

        return { (float) xPos, (float) yPos };
    }

    float DefaultInput::GetMouseXImpl()
    {
        Vector2 position = GetMousePositionImpl();
        return position.x;
    }

    float DefaultInput::GetMouseYImpl()
    {
        Vector2 position = GetMousePositionImpl();
        return position.y;
    }
}

