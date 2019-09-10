#pragma once

#include <Alpha/Window/Window.h>

#include <Alpha/Core/CoreMinimal.h>

#include <Alpha/ThirdParty/glad/glad.h>
#include <GLFW/glfw3.h>

namespace Alpha
{
	class DefaultWindowImpl : public Window
	{
	public:
		/// Constructor.
		explicit DefaultWindowImpl(const WindowProps& props = {"Alpha Window", 1280, 720 })
		{
		    m_props.title = props.title;
		    m_props.width = props.width;
		    m_props.height = props.height;
			Init();

            Logger::Info("Use engine default window implementation");
		}

		/// Destructor.
		~DefaultWindowImpl() override { Shutdown(); }

        inline void SetEventCallback(const EventCallbackFn& callback) override { m_props.eventCallback = callback; }

		/// Initialize the window.
		void Init();

		void Shutdown() override;

		void OnUpdate() override;

		uint32 GetWidth() const override { return m_props.width; }

		uint32 GetHeight() const override { return m_props.height; }

        void* GetNativeWindow() const override { return m_window; }

	private:
		/// Setup the alpha engine events callback 
		/// (i.e resize event, mouse events, ...etc).
		void SetupEventsCallback();

	private:
		GLFWwindow* m_window = nullptr;

		struct AlphaWindowProps : public WindowProps
        {
            EventCallbackFn eventCallback;
        };

        AlphaWindowProps m_props;
	};
}
