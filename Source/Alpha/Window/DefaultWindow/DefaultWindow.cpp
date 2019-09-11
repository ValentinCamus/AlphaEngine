#include "DefaultWindow.h"

#include <Alpha/Engine/EngineMinimal.h>

#include <Alpha/Event/ApplicationEvent.h>
#include <Alpha/Event/KeyEvent.h>
#include <Alpha/Event/MouseEvent.h>

#include <Alpha/Engine/Renderer/Renderer.h>

namespace Alpha
{
    struct Glfw
    {
        static bool bIsInitialized;
        static uint32 nInstance;
    };
    bool Glfw::bIsInitialized = false;
    uint32 Glfw::nInstance = 0;

    void DefaultWindow::Init()
    {
        Logger::Info("Creating {0}, Dimension = [{1}, {2}]", m_props.title, m_props.width, m_props.height);

        if (!Glfw::bIsInitialized)
        {
            int32 success = glfwInit();
            ALPHA_ASSERT(success, "Failed to initialize GLFW");
            Glfw::bIsInitialized = true;
        }
        ++Glfw::nInstance;

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
        glfwWindowHint32(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Requiered by on MacOs.
#endif

        m_window = glfwCreateWindow(m_props.width, m_props.height, ToCharArray(m_props.title), nullptr, nullptr);
        ALPHA_ASSERT(m_window, "Failed to create GLFW window");

        glfwMakeContextCurrent(m_window);
        glfwSetWindowUserPointer(m_window, &m_props);

        Alpha::Renderer::CreateContext();

        SetupEventsCallback();
    }

    void DefaultWindow::Shutdown()
    {
        --Glfw::nInstance;

        if (m_window) glfwDestroyWindow(m_window);
        if (Glfw::nInstance == 0) glfwTerminate();
    }

    void DefaultWindow::OnUpdate()
    {
        AppTickEvent event;
        m_props.eventCallback(event);

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }

    void DefaultWindow::SetupEventsCallback()
    {
        glfwSetWindowSizeCallback(m_window, [](GLFWwindow *window, int32 width, int32 height)
        {
            AlphaWindowProps &data = *(AlphaWindowProps *) glfwGetWindowUserPointer(window);
            data.width = width;
            data.height = height;

            WindowResizeEvent event(width, height);
            data.eventCallback(event);
        });

        glfwSetWindowCloseCallback(m_window, [](GLFWwindow *window)
        {
            AlphaWindowProps &data = *(AlphaWindowProps *) glfwGetWindowUserPointer(window);
            WindowCloseEvent event;
            data.eventCallback(event);
        });

        glfwSetKeyCallback(m_window, [](GLFWwindow *window, int32 key, int32 scanCode, int32 action, int32 mods)
        {
            AlphaWindowProps &data = *(AlphaWindowProps *) glfwGetWindowUserPointer(window);

            switch (action)
            {
                case GLFW_PRESS:
                {
                    KeyPressedEvent event(key, 0);
                    data.eventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent event(key);
                    data.eventCallback(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyPressedEvent event(key, 1);
                    data.eventCallback(event);
                    break;
                }
                default:
                    break;
            }
        });

        glfwSetCharCallback(m_window, [](GLFWwindow *window, unsigned int keyCode)
        {
            AlphaWindowProps &data = *(AlphaWindowProps *) glfwGetWindowUserPointer(window);

            KeyTypedEvent event(keyCode);
            data.eventCallback(event);
        });

        glfwSetMouseButtonCallback(m_window, [](GLFWwindow *window, int32 button, int32 action, int32 mods)
        {
            AlphaWindowProps &data = *(AlphaWindowProps *) glfwGetWindowUserPointer(window);

            switch (action)
            {
                case GLFW_PRESS:
                {
                    MouseButtonPressedEvent event(button);
                    data.eventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    MouseButtonReleasedEvent event(button);
                    data.eventCallback(event);
                    break;
                }
                default:
                    break;
            }
        });

        glfwSetScrollCallback(m_window, [](GLFWwindow *window, double xOffset, double yOffset)
        {
            AlphaWindowProps &data = *(AlphaWindowProps *) glfwGetWindowUserPointer(window);

            MouseScrolledEvent event((float) xOffset, (float) yOffset);
            data.eventCallback(event);
        });

        glfwSetCursorPosCallback(m_window, [](GLFWwindow *window, double xPos, double yPos)
        {
            AlphaWindowProps &data = *(AlphaWindowProps *) glfwGetWindowUserPointer(window);

            MouseMovedEvent event((float) xPos, (float) yPos);
            data.eventCallback(event);
        });
    }
}