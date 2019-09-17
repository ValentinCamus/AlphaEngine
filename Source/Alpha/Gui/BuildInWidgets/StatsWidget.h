#pragma once

#include <Alpha/Core/CoreMinimal.h>

#include <Alpha/ThirdParty/imgui/imgui.h>
#include <Alpha/Gui/ImGuiWidget.h>

#include <Alpha/Event/ApplicationEvent.h>

namespace Alpha
{
    constexpr uint32 MAX_HISTORY_SIZE = 256;
    constexpr uint32 MAX_FPS = 128;

    class StatsWidget : public ImGuiWidget
    {
    public:
        explicit StatsWidget(const std::string &name) : ImGuiWidget(name) { Init(); }

        void Init();
        void Render() override;

    private:
        uint32 m_cursor = 0;
        std::array<float, MAX_HISTORY_SIZE> m_fpsHistory;
    };
}