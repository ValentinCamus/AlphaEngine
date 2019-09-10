#pragma once

#include <Alpha/ThirdParty/imgui/imgui.h>

#include <Alpha/Layer/Layer.h>
#include <Alpha/Gui/ImGuiLayer.h>

#include <Alpha/Engine/Renderer/Renderer.h>

#define SANDBOX_CLEAR_COLOR {0.2f, 0.3f, 0.3f, 1.0f}

namespace Alpha
{
    class SandboxLayer : public Layer
    {
    public:
        explicit inline SandboxLayer() : Layer("Sandbox Layer") {}

        inline void OnUpdate() override
        {
            Renderer::Clear();
            Renderer::SetClearColor(SANDBOX_CLEAR_COLOR);


        }
    };

    class GuiSandboxLayer : public ImGuiLayer
    {
    public:
        explicit inline GuiSandboxLayer() : ImGuiLayer("Gui Sandbox Layer") {}

        inline void OnImGuiRender() override
        {
            ShowDockSpace();

            static bool show = true;
            ImGui::ShowDemoWindow(&show);
        }

    private:
        inline void ShowDockSpace()
        {
            bool bOptFullscreen = m_bIsDockSpaceFullscreen;
            static ImGuiDockNodeFlags dockSpaceFlags = ImGuiDockNodeFlags_None;

            // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dock-able into,
            // because it would be confusing to have two docking targets within each others.
            ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

            if (bOptFullscreen)
            {
                ImGuiViewport* viewport = ImGui::GetMainViewport();
                ImGui::SetNextWindowPos(viewport->Pos);
                ImGui::SetNextWindowSize(viewport->Size);
                ImGui::SetNextWindowViewport(viewport->ID);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
                windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
                windowFlags |= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
                windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
            }

            // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
            // and handle the pass-thru hole, so we ask Begin() to not render a background.
            if (dockSpaceFlags & ImGuiDockNodeFlags_PassthruCentralNode) windowFlags |= ImGuiWindowFlags_NoBackground;

            // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
            // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
            // all active windows docked into it will lose their parent and become un-docked.
            // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
            // any change of dock space/settings would lead to windows being stuck in limbo and never being visible.
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("DockSpace", &m_bShowDockSpace, windowFlags);
            ImGui::PopStyleVar();

            if (bOptFullscreen) ImGui::PopStyleVar(2);

            // DockSpace
            ImGuiIO& io = ImGui::GetIO();
            ALPHA_ASSERT(io.ConfigFlags & ImGuiConfigFlags_DockingEnable, "ImGui: Docking is not enabled");
            ImGuiID dockSpaceId = ImGui::GetID("DockSpace");
            ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), dockSpaceFlags);

            ImGui::End();
        }

    private:
        bool m_bShowDockSpace = true;
        bool m_bIsDockSpaceFullscreen = true;
    };
}
