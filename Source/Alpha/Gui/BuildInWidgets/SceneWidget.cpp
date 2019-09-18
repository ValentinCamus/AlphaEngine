#include "SceneWidget.h"

namespace Alpha
{
    void SceneWidget::Init()
    {

    }

    void SceneWidget::Render()
    {
        ImGui::SetWindowPos(ImVec2(64, 64), ImGuiCond_FirstUseEver);
        ImGui::SetWindowSize(ImVec2(512, 512), ImGuiCond_FirstUseEver);

        if (!ImGui::Begin(ToCharArray(GetName()), &m_bIsVisible))
        {
            // Early out if the window is collapsed, as an optimization.
            ImGui::End();
            return;
        }

        ImGui::Text("Scene Entities: ");
        ImGui::ListBox("", &m_selectedEntityIndex, m_entities.data(), m_entities.size(), 10);

        ImGui::End();
    }

    void SceneWidget::PushEntity(const char* name)
    {
        m_entities.push_back(name);
    }
}