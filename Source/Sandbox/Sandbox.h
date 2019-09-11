#pragma once

#include <Alpha/ThirdParty/imgui/imgui.h>

#include <Alpha/Layer/Layer.h>
#include <Alpha/Gui/ImGuiLayer.h>

#include <Alpha/Engine/Renderer/Renderer.h>
#include <Alpha/Engine/Renderer/Framebuffer.h>

#include <Alpha/Gui/BuildInWidgets/DockerWidget.h>
#include <Alpha/Gui/BuildInWidgets/ViewportWidget.h>

#define SANDBOX_CLEAR_COLOR {0.2f, 0.3f, 0.3f, 1.0f}


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
            m_framebuffer02 = Framebuffer::Create(500, 500);
            m_framebuffer03 = Framebuffer::Create(500, 500);

            m_viewport01.SetFramebuffer(m_framebuffer01);
            m_viewport02.SetFramebuffer(m_framebuffer02);
            m_viewport03.SetFramebuffer(m_framebuffer03);
        }

        inline void OnImGuiRender() override
        {
            m_docker.Render();

            m_framebuffer01->Bind();
            Renderer::SetClearColor({1, 0, 0, 1});
            Renderer::Clear();
            m_framebuffer01->Unbind();
            m_framebuffer02->Bind();
            Renderer::SetClearColor({0, 1, 0, 1});
            Renderer::Clear();
            m_framebuffer02->Unbind();
            m_framebuffer03->Bind();
            Renderer::SetClearColor({0, 0, 1, 1});
            Renderer::Clear();
            m_framebuffer03->Unbind();

            m_viewport01.Render();
            m_viewport02.Render();
            m_viewport03.Render();
        }

    private:
        DockerWidget m_docker = DockerWidget("Docker");
        ViewportWidget m_viewport01 = ViewportWidget("Viewport 01");
        ViewportWidget m_viewport02 = ViewportWidget("Viewport 02");
        ViewportWidget m_viewport03 = ViewportWidget("Viewport 03");

        Pointer<Framebuffer> m_framebuffer01 = nullptr;
        Pointer<Framebuffer> m_framebuffer02 = nullptr;
        Pointer<Framebuffer> m_framebuffer03 = nullptr;

    };
}
