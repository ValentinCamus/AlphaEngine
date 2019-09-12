#include "ViewportWidget.h"

namespace Alpha
{
    void ViewportWidget::Render()
    {
        if (!m_framebuffer)
        {
            Logger::Error("ViewportWidget::Render: ViewportWidget: {0}, Invalid framebuffer", GetName());
            return;
        }

        ImVec2 m_framebufferSize = ImVec2((float)m_framebuffer->GetWidth(), (float)m_framebuffer->GetHeight());

        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(m_framebufferSize.x + 16.f, m_framebufferSize.y + 36.f), ImGuiCond_FirstUseEver);

        // Main body of the Demo window starts here.
        if (!ImGui::Begin(ToCharArray(GetName()), &m_bIsVisible))
        {
            // Early out if the window is collapsed, as an optimization.
            ImGui::End();
            return;
        }

        ImVec2 winSize = ImGui::GetWindowSize();
        ImGui::Image((void*) m_framebuffer->GetTexture()->GetId(), m_framebufferSize, ImVec2(0, 0), ImVec2(1, 1));

        m_framebuffer->Bind();
        if (winSize.x != m_framebuffer->GetWidth() || winSize.y != m_framebuffer->GetHeight())
        {
            m_framebuffer->GetTexture()->Bind(0);
            m_framebuffer->Resize((uint32) winSize.x -16, (uint32) winSize.y - 36);
            m_framebuffer->GetTexture()->Unbind();
        }
        m_framebuffer->Unbind();

        ImGui::End();
    }
}