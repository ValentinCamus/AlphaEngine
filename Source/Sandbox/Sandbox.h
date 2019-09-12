#pragma once

#include <Alpha/ThirdParty/imgui/imgui.h>

#include <Alpha/Layer/Layer.h>
#include <Alpha/Gui/ImGuiLayer.h>

#include <Alpha/Engine/Renderer/Renderer.h>
#include <Alpha/Engine/Renderer/Framebuffer.h>

#include <Alpha/Gui/BuildInWidgets/DockerWidget.h>
#include <Alpha/Gui/BuildInWidgets/ViewportWidget.h>

#include <Alpha/Input/Input.h>

#include <Alpha/Engine/Camera/EulerCamera.h>

namespace Alpha
{
    class SandboxLayer : public Layer
    {
    public:
        explicit inline SandboxLayer() : Layer("Sandbox Layer") {}

        inline void OnUpdate() override
        {
            // AlphaEngine supports only US keyboard binding (aka QWERTY)
            if (Input::IsKeyPressed(ALPHA_KEY_W)) m_camera.MoveForward(1);
            if (Input::IsKeyPressed(ALPHA_KEY_S)) m_camera.MoveForward(-1);
            if (Input::IsKeyPressed(ALPHA_KEY_A)) m_camera.MoveRight(-1);
            if (Input::IsKeyPressed(ALPHA_KEY_D)) m_camera.MoveRight(1);


        }

    private:
        EulerCamera m_camera;
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

        inline void OnEvent(Event& e) override
        {
            m_docker.OnEvent(e);
            m_viewport01.OnEvent(e);
        }

    private:
        DockerWidget m_docker = DockerWidget("Docker");

        Pointer<Framebuffer> m_framebuffer01 = nullptr;
        ViewportWidget m_viewport01 = ViewportWidget("Viewport 01");
    };
}
