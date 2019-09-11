#include "Application.h"

#include <Alpha/Window/DefaultWindow/DefaultWindow.h>

namespace Alpha
{
    Application* Application::s_instance = nullptr;

    Application::Application()
    {
        ALPHA_ASSERT(!s_instance, "Application already exists!");
        s_instance = this;

        Pointer<DefaultWindow> window = NewPointer<DefaultWindow>();
        SetWindow(window);
    }

	void Application::Run()
	{
		m_bIsRunning = true;

		while (m_bIsRunning)
		{
			if (m_window) m_window->OnUpdate();
		}
	}

    void Application::SetWindow(const Pointer<Window> &window)
    {
	    m_window = window;
        m_window->SetEventCallback(ALPHA_BIND_EVENT(Application::OnEvent, this));
    }

    void Application::OnEvent(Event &e)
    {
        auto * dispatcher = new EventDispatcher(e);
        dispatcher->Dispatch<WindowCloseEvent>(ALPHA_BIND_EVENT(Application::OnWindowClose, this));
        dispatcher->Dispatch<AppTickEvent>(ALPHA_BIND_EVENT(Application::OnUpdate, this));

        for (const Pointer<Layer>& layer : m_layerStack) layer->OnEvent(e);

    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        if (m_bIsRunning)
        {
            m_bIsRunning = false;
            m_window = nullptr;
            return true;
        }

        Logger::Warn("Trying to stop an invalid application - Nothing happened");
        return false;
    }

    bool Application::OnUpdate(AppTickEvent &e)
    {
        for (const Pointer<Layer>& layer : m_layerStack) layer->OnUpdate();
        return true;
    }
}