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
#include <Alpha/Engine/Renderer/Shader.h>
#include <Alpha/Engine/StaticMeshModel/StaticMeshModel.h>
#include <Alpha/Engine/StaticMeshModel/StaticMeshEntity.h>
namespace Alpha
{
    static Pointer<Framebuffer> m_framebuffer01 = nullptr;

    class SandboxLayer : public Layer
    {
    public:
        explicit inline SandboxLayer() : Layer("Sandbox Layer") { Init(); }

        inline void Init()
        {
            m_shader = Shader::Create(PROJECT_SOURCE_DIR + "Shaders/MeshShader.glsl");

            m_framebuffer01 = Framebuffer::Create(500, 500);

            std::vector<Vertex> vertices = {
                    Vertex({  0.5f,  0.5f, 0.0f }),  // top right
                    Vertex({  0.5f, -0.5f, 0.0f }),  // bottom right
                    Vertex({ -0.5f, -0.5f, 0.0f }),  // bottom left
                    Vertex({ -0.5f,  0.5f, 0.0f })   // top left
            };

            std::vector<uint32> indices = {
                    0, 1, 3,   // first triangle
                    1, 2, 3    // second triangle
            };

            m_sm = NewPointer<StaticMeshModel>();
            m_sm->Load(vertices, indices);

            m_entity = NewPointer<StaticMeshEntity>("Entity", m_sm);

            Pointer<Material> material = NewPointer<Material>();
            material->SetKd(Vector4(1.0f));
            material->SetKs(Vector4(1.0f));
            material->SetRoughness(0.9f);

            m_entity->SetMaterial(0, material);

            m_entity->SetWorldRotation({0, 0, 45});
            m_entity->SetWorldLocation({0, 0, -2});
        }

        inline void OnUpdate() override
        {
            // AlphaEngine supports only US keyboard binding (aka QWERTY)
            if (Input::IsKeyPressed(ALPHA_KEY_W)) m_camera.MoveForward(1);
            if (Input::IsKeyPressed(ALPHA_KEY_S)) m_camera.MoveForward(-1);
            if (Input::IsKeyPressed(ALPHA_KEY_A)) m_camera.MoveRight(-1);
            if (Input::IsKeyPressed(ALPHA_KEY_D)) m_camera.MoveRight(1);

            ALPHA_ASSERT(m_framebuffer01, "Invalid Framebuffer: 01");

            m_framebuffer01->Bind();
            m_shader->Bind();

            Renderer::Clear();
            Renderer::SetClearColor({0.2f, 0.3f, 0.3f, 1.0f});

            float fb01AspectRatio = (float)m_framebuffer01->GetWidth() / (float)m_framebuffer01->GetHeight();
            Matrix4x4 projectionMatrix = MakeProjectionMatrix(m_camera.GetZoom(), fb01AspectRatio);

            TransformMatrix transformMatrix = { Matrix4x4(1), m_camera.GetViewMatrix(), projectionMatrix };

            m_entity->Draw(m_shader, transformMatrix);

            m_shader->Unbind();
            m_framebuffer01->Unbind();
        }

    private:
        EulerCamera m_camera;
        Pointer<Shader> m_shader;

        Pointer<StaticMeshModel> m_sm;
        Pointer<StaticMeshEntity> m_entity;
    };

    class GuiSandboxLayer : public ImGuiLayer
    {
    public:
        explicit inline GuiSandboxLayer() : ImGuiLayer("Gui Sandbox Layer")
        {
            m_viewport01.SetFramebuffer(m_framebuffer01);
        }

        inline void OnImGuiRender() override
        {
            m_docker.Render();
            m_viewport01.Render();
        }

        inline void OnEvent(Event& e) override
        {
            m_docker.OnEvent(e);
            m_viewport01.OnEvent(e);
        }

    private:
        DockerWidget m_docker = DockerWidget("Docker");
        ViewportWidget m_viewport01 = ViewportWidget("Viewport 01");
    };
}
