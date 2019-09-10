#pragma once

#include "Event.h"

#include <sstream>

#include <Alpha/Core/CoreMinimal.h>

namespace Alpha {

    class KeyEvent : public Event
    {
    public:
        inline int32 GetKeyCode() const { return m_keyCode; }

        inline uint32 GetCategoryFlags() const override
        {
            return (uint32) EventCategory::EventCategoryKeyboard | (uint32) EventCategory::EventCategoryInput;
        }

    protected:
        explicit KeyEvent(int32 keyCode) : m_keyCode(keyCode) {}

    protected:
        int32 m_keyCode;
    };

    class KeyPressedEvent : public KeyEvent 
    {
    public:
        KeyPressedEvent(int32 keyCode, int32 repeatCount) : KeyEvent(keyCode), m_repeatCount(repeatCount) {}

        inline int32 GetRepeatCount() const { return m_repeatCount; }

        inline std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyPressedEvent: " << m_keyCode << " (" << m_repeatCount << " repeats)";
            return ss.str();
        }

        inline uint32 GetCategoryFlags() const override
        {
            return (uint32) EventCategory::EventCategoryKeyboard | (uint32) EventCategory::EventCategoryInput;
        }

        inline static EventType GetStaticType() { return EventType::KeyPressed; }
        inline EventType GetEventType() const override { return GetStaticType(); }
        inline const char* GetName() const override { return "KeyPressed"; }

    private:
        int32 m_repeatCount;
    };

    class KeyReleasedEvent : public KeyEvent
    {
    public:
        explicit KeyReleasedEvent(int32 keyCode) : KeyEvent(keyCode) {}

        inline std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyReleasedEvent: " << m_keyCode;
            return ss.str();
        }

        inline static EventType GetStaticType() { return EventType::KeyReleased; }
        inline EventType GetEventType() const override { return GetStaticType(); }
        inline const char* GetName() const override { return "KeyReleased"; }
    };

    class KeyTypedEvent : public KeyEvent
    {
    public:
        explicit KeyTypedEvent(int32 keyCode) : KeyEvent(keyCode) {}

        inline std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyTypedEvent: " << m_keyCode;
            return ss.str();
        }

        inline static EventType GetStaticType() { return EventType::KeyTyped; }
        inline EventType GetEventType() const override { return GetStaticType(); }
        inline const char* GetName() const override { return "KeyTyped"; }
    };
}
