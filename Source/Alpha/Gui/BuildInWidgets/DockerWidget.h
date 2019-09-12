#pragma once

#include <Alpha/Core/CoreMinimal.h>

#include <Alpha/ThirdParty/imgui/imgui.h>
#include <Alpha/Gui/ImGuiWidget.h>

namespace Alpha
{
    class DockerWidget : public ImGuiWidget
    {
    public:
        using ImGuiWidget::ImGuiWidget;

        void Render() override;

    private:
        bool m_bIsDockSpaceFullscreen = true;
    };
}