#pragma once

#include <Alpha/ThirdParty/imgui/imgui.h>

#include <Alpha/Layer/Layer.h>
#include <Alpha/Gui/ImGuiLayer.h>

#include <Alpha/Engine/Renderer/Renderer.h>
#include <Alpha/Engine/Renderer/Framebuffer.h>

#include <Alpha/Gui/BuildInWidgets/DockerWidget.h>
#include <Alpha/Gui/BuildInWidgets/ViewportWidget.h>

namespace Alpha
{
    class SandboxLayer : public Layer
    {
    public:
        explicit inline SandboxLayer() : Layer("Sandbox Layer") {}

        inline void OnUpdate() override {}
    };

    class GuiSandboxLayer : public ImGuiLayer
    {
    public:
        explicit inline GuiSandboxLayer() : ImGuiLayer("Gui Sandbox Layer")
        {
            m_framebuffer01 = Framebuffer::Create(500, 500);
            m_viewport01.SetFramebuffer(m_framebuffer01);
        }

        inline void OnImGuiRender() override
        {
            m_docker.Render();

            // Debug only
            m_framebuffer01->Bind();
            Renderer::Clear();
            Renderer::SetClearColor({0.2f, 0.3f, 0.3f, 1.0f});
            m_framebuffer01->Unbind();

            m_viewport01.Render();
        }

    private:
        DockerWidget m_docker = DockerWidget("Docker");

        Pointer<Framebuffer> m_framebuffer01 = nullptr;
        ViewportWidget m_viewport01 = ViewportWidget("Viewport 01");
    };
}
