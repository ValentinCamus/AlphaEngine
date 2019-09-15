#include "Input.h"

#include <Alpha/Input/GlfwInput/GlfwInput.h>

namespace Alpha
{
    Input* Input::s_instance = nullptr;

    void Input::Init()
    {
        ALPHA_ASSERT(!s_instance, "Input system already instantiated");

        Logger::Info("Initializing: AlphaEngine input system...");
        s_instance = new GlfwInput;
        Logger::Info("AlphaEngine input system initialized");
    }
}