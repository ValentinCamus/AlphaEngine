#include "OpenGLRenderer.h"

#include <GLFW/glfw3.h>

namespace Alpha
{
    void OpenGLRendererAPI::InitImpl()
    {
        Logger::Info("OpenGL Renderer initialization [OK]");
    }

    void OpenGLRendererAPI::CreateContextImpl()
    {
        ALPHA_ASSERT(gladLoadGLLoader((GLADloadproc) glfwGetProcAddress), "Failed to initialize GLAD");

        Logger::Info("(OpenGL) Vendor: {0}", glGetString(GL_VENDOR));
        Logger::Info("(OpenGL) Renderer: {0}", glGetString(GL_RENDERER));
        Logger::Info("(OpenGL) Version: {0}", glGetString(GL_VERSION));

        GL_CHECK(glEnable(GL_DEPTH_TEST));
        // GL_CHECK(glEnable(GL_BLEND));
        // GL_CHECK(glEnable(GL_CULL_FACE));

        // GL_CHECK(glFrontFace(GL_CW));
        // GL_CHECK(glCullFace(GL_FRONT));

        // GL_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    }

    void OpenGLRendererAPI::SetClearColorImpl(const Color4 &color)
    {
        GL_CHECK(glClearColor(color.r, color.g, color.b, color.a));
    }

    void OpenGLRendererAPI::ClearImpl()
    {
        GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    }
}

