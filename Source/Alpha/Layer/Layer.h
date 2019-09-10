#pragma once

#include <Alpha/Core/CoreMinimal.h>

#include <Alpha/Event/Event.h>

namespace Alpha
{
    class Layer
    {
    public:
        explicit Layer(const std::string& name = "Layer") : m_name(name)
        {
            Logger::Info("Creating layer: {0}", name);
        }

        virtual ~Layer() = default;

        virtual void OnAttach() {}
        virtual void OnDetach() {}

        virtual void OnUpdate() {}
        virtual void OnEvent(Event& event) {}

        inline const std::string& GetName() const { return m_name; }
    protected:
        std::string m_name;
    };
}
