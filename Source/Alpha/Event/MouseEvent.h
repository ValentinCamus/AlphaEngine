#pragma once

#include "Event.h"

#include <sstream>

#include <Alpha/Core/CoreMinimal.h>

namespace Alpha {

    class MouseMovedEvent : public Event 
    {
    public:
        MouseMovedEvent(float x, float y) : m_mouseX(x), m_mouseY(y) {}

        inline float GetX() const { return m_mouseX; }
        inline float GetY() const { return m_mouseY; }

        std::string ToString() const override 
        {
            std::stringstream ss;
            ss << "MouseMovedEvent: " << m_mouseX << ", " << m_mouseY;
            return ss.str();
        }

        inline static EventType GetStaticType() { return EventType::MouseMoved; }
        inline EventType GetEventType() const override { return GetStaticType(); }
        inline const char* GetName() const override { return "MouseMoved"; }

        inline uint32 GetCategoryFlags() const override 
        {
            return (uint32) EventCategory::EventCategoryMouse | (uint32) EventCategory::EventCategoryInput;
        }

    private:
        float m_mouseX, m_mouseY;
    };

    class MouseScrolledEvent : public Event 
    {
    public:
        MouseScrolledEvent(float xOffset, float yOffset) : m_xOffset(xOffset), m_yOffset(yOffset) {}

        inline float GetXOffset() const { return m_xOffset; }
        inline float GetYOffset() const { return m_yOffset; }

        std::string ToString() const override 
        {
            std::stringstream ss;
            ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
            return ss.str();
        }

        inline static EventType GetStaticType() { return EventType::MouseScrolled; }
        inline EventType GetEventType() const override { return GetStaticType(); }
        inline const char* GetName() const override { return "MouseScrolled"; }

        inline uint32 GetCategoryFlags() const override 
        {
            return (uint32) EventCategory::EventCategoryMouse | (uint32) EventCategory::EventCategoryInput;
        }

    private:
        float m_xOffset, m_yOffset;
    };

    class MouseButtonEvent : public Event 
    {
    public:
        inline int32 GetMouseButton() const { return m_button; }

        inline uint32 GetCategoryFlags() const override
        {
            return (uint32) EventCategory::EventCategoryMouse | (uint32) EventCategory::EventCategoryInput;
        }

    protected:
        explicit MouseButtonEvent(int32 button) : m_button(button) {}

        int32 m_button;
    };

    class MouseButtonPressedEvent : public MouseButtonEvent 
    {
    public:
        explicit MouseButtonPressedEvent(int32 button) : MouseButtonEvent(button) {}

        std::string ToString() const override 
        {
            std::stringstream ss;
            ss << "MouseButtonPressedEvent: " << m_button;
            return ss.str();
        }

        inline static EventType GetStaticType() { return EventType::MouseButtonPressed; }
        inline EventType GetEventType() const override { return GetStaticType(); }
        inline const char* GetName() const override { return "MouseButtonPressed"; }

    };

    class MouseButtonReleasedEvent : public MouseButtonEvent 
    {
    public:
        explicit MouseButtonReleasedEvent(int32 button) : MouseButtonEvent(button) {}

        std::string ToString() const override 
        {
            std::stringstream ss;
            ss << "MouseButtonReleasedEvent: " << m_button;
            return ss.str();
        }

        inline static EventType GetStaticType() { return EventType::MouseButtonReleased; }
        inline EventType GetEventType() const override { return GetStaticType(); }
        inline const char* GetName() const override { return "MouseButtonReleased"; }
    };

}