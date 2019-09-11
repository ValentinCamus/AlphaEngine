#pragma once

#include <Alpha/Core/CoreMinimal.h>

namespace Alpha
{
    class ImGuiWidget
    {
    public:
        explicit ImGuiWidget(const std::string& name) : m_name(name)
        {
            Logger::Info("Creating ImGuiWidget: {0}", name);
        }

        virtual void Render() = 0;

        virtual bool IsVisible() const { return m_bIsVisible; }
        virtual void SetVisibility(bool bIsVisible) { m_bIsVisible = bIsVisible; }

        virtual const std::string& GetName() const { return m_name; }
        virtual void SetName(const std::string& name) { m_name = name; }

    protected:
        bool m_bIsVisible = true;

    private:
        std::string m_name;
    };
}