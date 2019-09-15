#pragma once

#include <Alpha/Window/Window.h>

#include <Alpha/Core/CoreMinimal.h>

#include <Alpha/ThirdParty/glad/glad.h>
#include <GLFW/glfw3.h>

namespace Alpha
{
	class GlfwWindow : public Window
	{
	public:
		/// Constructor.
		explicit GlfwWindow(const WindowProps& props)
		{
		    m_props.title = props.title;
		    m_props.width = props.width;
		    m_props.height = props.height;
			Init();
		}

		/// Destructor.
		~GlfwWindow() override { Shutdown(); }

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
