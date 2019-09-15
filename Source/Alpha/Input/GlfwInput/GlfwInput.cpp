#include "GlfwInput.h"

#include <Alpha/Application/Application.h>
#include <GLFW/glfw3.h>
#include <Alpha/Input/Input.h>


namespace Alpha
{
    GlfwInput::GlfwInput()
    {
        Logger::Info("Input system: Use GLFW input system");
        Logger::Warn("AlphaEngine supports only US keyboard binding (aka QWERTY)");
    }

    bool GlfwInput::IsKeyPressedImpl(int keyCode)
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetNativeWindow());
        auto state = glfwGetKey(window, keyCode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool GlfwInput::IsMouseButtonPressedImpl(int button)
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetNativeWindow());
        auto state = glfwGetMouseButton(window, button);
        return state == GLFW_PRESS;
    }

    Vector2 GlfwInput::GetMousePositionImpl()
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetNativeWindow());
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);

        return { (float) xPos, (float) yPos };
    }

    float GlfwInput::GetMouseXImpl()
    {
        Vector2 position = GetMousePositionImpl();
        return position.x;
    }

    float GlfwInput::GetMouseYImpl()
    {
        Vector2 position = GetMousePositionImpl();
        return position.y;
    }
}

