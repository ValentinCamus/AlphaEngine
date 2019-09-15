#pragma once

#include <Alpha/Core/CoreMinimal.h>
#include <Alpha/Event/Event.h>

namespace Alpha
{
    struct WindowProps
    {
        /// The title of the window.
        std::string title;
        /// The width of the window.
        uint32 width;
        /// The height of the window.
        uint32 height;
    };

    class Window
	{
    public:
        static Pointer<Window> Create(const WindowProps& props);

	public:
        using EventCallbackFn = std::function<void(Event&)>;

		/// Destructor.
		virtual ~Window() = default;

		/// Close the window.
		virtual void Shutdown() = 0;

		/// @setter: Set the event callback.
        virtual void SetEventCallback(const EventCallbackFn& callback) = 0;

		/// Event tick, call this function to update the window.
		virtual void OnUpdate() = 0;

		/// @getter: The width of the window.
		virtual uint32 GetWidth() const = 0;

		/// @getter: The height of the window.
		virtual uint32 GetHeight() const = 0;

		/// The native window (aka the window created by the library/framework).
        virtual void* GetNativeWindow() const = 0;
	};
}