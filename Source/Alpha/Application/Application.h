#pragma once

#include <Alpha/Core/CoreMinimal.h>
#include <Alpha/Window/Window.h>

#include <Alpha/Event/Event.h>

#include <Alpha/Event/ApplicationEvent.h>
#include <Alpha/Event/KeyEvent.h>
#include <Alpha/Event/MouseEvent.h>

#include <Alpha/Layer/Layer.h>
#include <Alpha/Layer/LayerStack.h>

namespace Alpha
{
	class Application
	{
	public:
	    explicit Application(const Pointer<Window>& window = nullptr);

		void Run();

		inline Pointer<Window> GetWindow() { return m_window; }
		void SetWindow(const Pointer<Window>& window);

        inline void PushLayer(const Pointer<Layer>& layer) { m_layerStack.PushLayer(layer); }

		/// @getter: The current instance of Application.
        inline static Application& Get() { return *s_instance; }

	public:
        void OnEvent(Event& e);
        bool OnWindowClose(WindowCloseEvent& e);
        bool OnUpdate(AppTickEvent& e);

	private:
		bool m_bIsRunning = false;
        LayerStack m_layerStack = LayerStack("Application Layer Stack");
		Pointer<Window> m_window = nullptr;

    private:
	    /// The current instance of Application.
        static Application* s_instance;
	};
}