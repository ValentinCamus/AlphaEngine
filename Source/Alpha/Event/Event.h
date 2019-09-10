#pragma once

#include <functional>

#include <Alpha/Core/CoreMinimal.h>

#include <Alpha/Event/InputCode/KeyCodes.h>
#include <Alpha/Event/InputCode/MouseCodes.h>

namespace Alpha
{
    enum class EventType
    {
        None = 0,
        WindowClose,
        WindowResize,
        WindowFocus,
        WindowLostFocus,
        WindowMoved,
        AppTick,
        KeyPressed,
        KeyReleased,
        KeyTyped,
        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved,
        MouseScrolled
    };

    enum class EventCategory : uint32
    {
        None = 0,
        EventCategoryApplication    = 1u << 0u,
        EventCategoryInput          = 1u << 1u,
        EventCategoryKeyboard       = 1u << 2u,
        EventCategoryMouse          = 1u << 3u,
        EventCategoryMouseButton    = 1u << 4u
    };

    class Event
    {
    public:
        bool bIsHandled = false;

        virtual EventType GetEventType() const = 0;
        virtual const char* GetName() const = 0;
        virtual uint32 GetCategoryFlags() const = 0;
        virtual std::string ToString() const { return GetName(); }

        /// Check if this event is in the given category.
        inline bool IsInCategory(EventCategory category) { return GetCategoryFlags() & (uint32)category; }
    };

    class EventDispatcher
    {
    public:
        explicit EventDispatcher(Event& event) : m_event(event) {}

        // F will be deduced by the compiler
        template<typename T, typename F>
        bool Dispatch(const F& func)
        {
            if (m_event.GetEventType() == T::GetStaticType())
            {
                m_event.bIsHandled = func(static_cast<T&>(m_event));
                return true;
            }
            return false;
        }

    private:
        Event& m_event;
    };

    inline std::ostream& operator<<(std::ostream& os, const Event& e)
    {
        return os << e.ToString();
    }
}