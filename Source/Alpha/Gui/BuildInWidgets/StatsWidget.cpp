#include "StatsWidget.h"

namespace Alpha
{
    void StatsWidget::Init()
    {
        std::fill(m_fpsHistory.begin(), m_fpsHistory.end(), 0.0f);
    }

    void StatsWidget::Render()
    {
        ImGui::SetWindowPos(ImVec2(64, 64), ImGuiCond_FirstUseEver);
        ImGui::SetWindowSize(ImVec2(512, 512), ImGuiCond_FirstUseEver);

        if (!ImGui::Begin(ToCharArray(GetName()), &m_bIsVisible))
        {
            // Early out if the window is collapsed, as an optimization.
            ImGui::End();
            return;
        }

        float fps = ImGui::GetIO().Framerate;
        m_fpsHistory[(m_cursor++) % MAX_HISTORY_SIZE] = fps;

        ImGui::Text("Frame per second: %f", fps);
        ImGui::PlotLines("Frame times", m_fpsHistory.data(), m_fpsHistory.size(), 0, nullptr, 0, MAX_FPS);

        ImGui::End();
    }
}