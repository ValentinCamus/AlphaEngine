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

		inline uint32 GetWidth() const override { return m_props.width; }

		inline uint32 GetHeight() const override { return m_props.height; }

        inline void* GetNativeWindow() const override { return m_window; }

        inline void SetFramerate(uint32 framerate) override { m_framerate = framerate; };

		uint32 GetFramerate() const override { return m_framerate; }

		void SetVSync(bool enabled) override;

		inline bool IsVSync() const override { return m_props.bIsVSync; }

	private:
		/// Setup the alpha engine events callback 
		/// (i.e resize event, mouse events, ...etc).
		void SetupEventsCallback();

	private:
		GLFWwindow* m_window = nullptr;

		struct AlphaWindowProps : public WindowProps
        {
            EventCallbackFn eventCallback;
			bool bIsVSync = true;
        };

        AlphaWindowProps m_props;

        uint32 m_framerate = Window::DEFAULT_FPS;

        double m_lastTime;
	};
}
