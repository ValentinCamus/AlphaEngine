#pragma once

#include "Event.h"

#include <sstream>

#include <Alpha/Core/CoreMinimal.h>

namespace Alpha
{

    class WindowResizeEvent : public Event
    {
    public:
        WindowResizeEvent(unsigned int width, unsigned int height) : m_width(width), m_height(height) {}

        inline unsigned int GetWidth() const { return m_width; }
        inline unsigned int GetHeight() const { return m_height; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "WindowResizeEvent: [width=" << m_width << ", height=" << m_height << "]";
            return ss.str();
        }

        inline static EventType GetStaticType() { return EventType::WindowResize; }
	    inline EventType GetEventType() const override { return GetStaticType(); }
	    inline const char* GetName() const override { return "WindowResize"; }

        inline uint32 GetCategoryFlags() const override { return (uint32)EventCategory::EventCategoryApplication; }

    private:
        uint32 m_width;
        uint32 m_height;
    };

    class WindowCloseEvent : public Event
    {
    public:
        inline static EventType GetStaticType() { return EventType::WindowClose; }
        inline EventType GetEventType() const override { return GetStaticType(); }
        inline const char* GetName() const override { return "WindowClose"; }
        inline uint32 GetCategoryFlags() const override { return (uint32) EventCategory::EventCategoryApplication; }
    };

    class AppTickEvent : public Event
    {
    public:
        inline static EventType GetStaticType() { return EventType::AppTick; }
        inline EventType GetEventType() const override { return GetStaticType(); }
        inline const char* GetName() const override { return "AppTick"; }
        inline uint32 GetCategoryFlags() const override { return (uint32) EventCategory::EventCategoryApplication; }
};
}
