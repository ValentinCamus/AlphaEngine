#include "OpenGLRenderer.h"

#include <GLFW/glfw3.h>

namespace Alpha
{
    void OpenGLRendererAPI::InitImpl()
    {
        // All glEnable functions

        Logger::Info("OpenGL Renderer initialization [OK]");
    }

    void OpenGLRendererAPI::CreateContextImpl()
    {
        ALPHA_ASSERT(gladLoadGLLoader((GLADloadproc) glfwGetProcAddress), "Failed to initialize GLAD");

        Logger::Info("(OpenGL) Vendor: {0}", glGetString(GL_VENDOR));
        Logger::Info("(OpenGL) Renderer: {0}", glGetString(GL_RENDERER));
        Logger::Info("(OpenGL) Version: {0}", glGetString(GL_VERSION));
    }

    void OpenGLRendererAPI::SetClearColorImpl(const Color4 &color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void OpenGLRendererAPI::ClearImpl()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}

