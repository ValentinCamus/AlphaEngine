#include "ViewportWidget.h"

#include <Alpha/Gui/ImGuiLayer.h>
#include <Alpha/Engine/Renderer/Renderer.h>

namespace Alpha
{
    void ViewportWidget::Render()
    {
        if (!m_framebuffer)
        {
            Logger::Error("ViewportWidget::Render: ViewportWidget: {0}, Invalid framebuffer", GetName());
            return;
        }

        ImVec2 fbSize = ImVec2((float)m_framebuffer->GetWidth(), (float)m_framebuffer->GetHeight());

        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(fbSize.x, fbSize.y), ImGuiCond_FirstUseEver);

        // Main body of the Demo window starts here.
        if (!ImGui::Begin(ToCharArray(GetName()), &m_bIsVisible))
        {
            // Early out if the window is collapsed, as an optimization.
            ImGui::End();
            return;
        }

        ImVec2 winSize = ImGui::GetWindowSize();

        m_framebuffer->Bind();

        if (winSize.x != m_framebuffer->GetWidth() || winSize.y != m_framebuffer->GetHeight())
        {
            m_framebuffer->GetTexture()->Bind(0);
            m_framebuffer->Resize((uint32) winSize.x, (uint32) winSize.y);
            m_framebuffer->GetTexture()->Unbind();
        }
        else
        {
            void* id = (void*)(uintptr_t) m_framebuffer->GetTexture()->GetId();
            ImGui::Image(id, winSize, ImVec2(0, 1), ImVec2(1, 0));
        }

        m_framebuffer->Unbind();

        ImGui::End();
    }
}