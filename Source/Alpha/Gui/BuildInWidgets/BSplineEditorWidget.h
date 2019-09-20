#pragma once

#include <Alpha/Core/CoreMinimal.h>

#include <Alpha/ThirdParty/imgui/imgui.h>
#include <Alpha/Gui/ImGuiWidget.h>

#include <Alpha/Event/ApplicationEvent.h>

namespace Alpha
{
    class BSplineEditor : public ImGuiWidget
    {
    public:
        explicit BSplineEditor(const std::string &name) : ImGuiWidget(name) {}

        void Render() override;
    };
}