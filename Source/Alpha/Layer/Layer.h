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

        virtual inline void OnAttach() {}
        virtual inline void OnDetach() {}

        virtual inline void OnUpdate() {}
        virtual inline void OnEvent(Event& event) { ALPHA_UNUSED(event); }

        virtual inline const std::string& GetName() const { return m_name; }

    protected:
        std::string m_name;
    };
}
