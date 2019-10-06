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

		m_components = { "<No Selection>" };

		if (m_scene && m_scene->GetNbComponents() > 0)
		{
			for (uint32 i = 0; i < m_scene->GetNbComponents(); ++i)
			{
				const std::string& name = m_scene->GetComponentAt(i)->GetName();
				m_components.push_back(ToCharArray(name));
			}
		}  

        ImGui::ListBox("", &m_selectedEntityIndex, m_components.data(), m_components.size(), 10);

        ImGui::End();
    }
}