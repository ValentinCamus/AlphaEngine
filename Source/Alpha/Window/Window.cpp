#include "Window.h"

#include <Alpha/Window/GlfwWindow/GlfwWindow.h>

namespace Alpha
{
    Pointer<Window> Window::Create(const WindowProps& props)
    {
        return NewPointer<GlfwWindow>(props);
    }
}