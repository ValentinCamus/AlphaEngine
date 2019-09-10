#include "DefaultInputImpl.h"

#include <Alpha/Application/Application.h>
#include <GLFW/glfw3.h>

namespace Alpha
{
    Input* Input::s_instance = new DefaultInputImpl();

    DefaultInputImpl::DefaultInputImpl()
    {
        Logger::Info("Use engine default inputs implementation");
        Logger::Warn("AlphaEngine supports only US keyboard binding (aka QWERTY)");
    }

    bool DefaultInputImpl::IsKeyPressedImpl(int keyCode)
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetNativeWindow());
        auto state = glfwGetKey(window, keyCode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool DefaultInputImpl::IsMouseButtonPressedImpl(int button)
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetNativeWindow());
        auto state = glfwGetMouseButton(window, button);
        return state == GLFW_PRESS;
    }

    Vector2 DefaultInputImpl::GetMousePositionImpl()
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetNativeWindow());
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);

        return { (float) xPos, (float) yPos };
    }

    float DefaultInputImpl::GetMouseXImpl()
    {
        Vector2 position = GetMousePositionImpl();
        return position.x;
    }

    float DefaultInputImpl::GetMouseYImpl()
    {
        Vector2 position = GetMousePositionImpl();
        return position.y;
    }
}

