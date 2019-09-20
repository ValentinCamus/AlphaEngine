#pragma once

#include <Alpha/ThirdParty/imgui/imgui.h>

#include <Alpha/Layer/Layer.h>
#include <Alpha/Gui/ImGuiLayer.h>

#include <Alpha/Engine/Renderer/Renderer.h>
#include <Alpha/Engine/Renderer/Framebuffer.h>

#include <Alpha/Gui/BuildInWidgets/StatsWidget.h>
#include <Alpha/Gui/BuildInWidgets/DockerWidget.h>
#include <Alpha/Gui/BuildInWidgets/ViewportWidget.h>
#include <Alpha/Gui/BuildInWidgets/SceneWidget.h>
#include <Alpha/Gui/BuildInWidgets/MaterialEditorWidget.h>

#include <Alpha/Input/Input.h>

#include <Alpha/Engine/Renderer/Shader.h>

#include <Alpha/Engine/Camera/EulerCamera.h>

#include <Alpha/Engine/Mesh/StaticMeshModel.h>
#include <Alpha/Engine/Mesh/StaticMeshEntity.h>

#include <Alpha/Engine/Renderer/Texture.h>
#include <Alpha/Engine/Material/Material.h>

namespace Alpha
{
    static Pointer<Framebuffer> s_framebuffer01 = nullptr;

    Pointer<StaticMeshEntity> s_entity = nullptr;

    class SandboxLayer : public Layer
    {
    public:
        explicit inline SandboxLayer() : Layer("Sandbox Layer") { Init(); }

        inline void Init()
        {
            m_shader = Shader::Create("Physical-Based-Rendering", {
                {Shader::GLSL_VERTEX_SHADER, PROJECT_SOURCE_DIR + "Shaders/Pbr.vs.glsl"},
                {Shader::GLSL_FRAGMENT_SHADER, PROJECT_SOURCE_DIR + "Shaders/Pbr.fs.glsl"}
            });

            s_framebuffer01 = Framebuffer::Create(500, 500);

            m_light = NewPointer<DirectionalLight>();

            m_sm = NewPointer<StaticMeshModel>();
            m_sm->Load(PROJECT_SOURCE_DIR + "Assets/StanfordDragon.fbx");

            m_entity = NewPointer<StaticMeshEntity>("Stanford Dragon 01", m_sm);
            s_entity = m_entity;

            m_texture = Texture2D::Create(PROJECT_SOURCE_DIR + "Assets/Brick.jpg");
            Pointer<Material> material01 = NewPointer<Material>("Material_01");
            Pointer<Material> material02 = NewPointer<Material>("Material_02");

            material01->SetKd(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
            material01->SetKs(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
            material01->SetRoughness(0.1f);

            material02->AddTexture(ETextureType::TX_Diffuse, m_texture);

            m_entity->SetMaterial(0, material01);
            m_entity->SetMaterial(1, material02);
            m_entity->SetWorldLocation({0, -1, -2});
            m_entity->SetWorldScale({0.05, 0.05, 0.05});
        }

        inline void OnUpdate() override
        {
            // AlphaEngine supports only US keyboard binding (aka QWERTY)
            if (Input::IsKeyPressed(ALPHA_KEY_W)) m_camera.MoveForward(1);
            if (Input::IsKeyPressed(ALPHA_KEY_S)) m_camera.MoveForward(-1);
            if (Input::IsKeyPressed(ALPHA_KEY_A)) m_camera.MoveRight(-1);
            if (Input::IsKeyPressed(ALPHA_KEY_D)) m_camera.MoveRight(1);

            if (Input::IsMouseButtonPressed(ALPHA_MOUSE_BUTTON_3)) m_camera.Look(Input::GetMousePosition());


            ALPHA_ASSERT(s_framebuffer01, "Invalid Framebuffer: 01");

            s_framebuffer01->Bind();
            m_shader->Bind();

            Renderer::Clear();
            Renderer::SetClearColor({0.2f, 0.3f, 0.3f, 1.0f});

            m_shader->SetUniform("nLights", 1);
            m_shader->SetUniform("lights[0]", m_light);

            float fb01AspectRatio = (float)s_framebuffer01->GetWidth() / (float)s_framebuffer01->GetHeight();
            Matrix4x4 projectionMatrix = MakeProjectionMatrix(m_camera.GetZoom(), fb01AspectRatio);

            Matrix4x4 viewMatrix = MakeViewMatrix(m_camera.GetWorldLocation(),  m_camera.GetWorldRotation());

            TransformMatrix transformMatrix = {Matrix4x4(1), viewMatrix, projectionMatrix};

            m_entity->Draw(m_shader, transformMatrix);

            m_shader->Unbind();
            s_framebuffer01->Unbind();
        }

    private:
        EulerCamera m_camera;
        Pointer<Shader> m_shader;

        Pointer<StaticMeshModel> m_sm;
        Pointer<StaticMeshEntity> m_entity;

        Pointer<Texture2D> m_texture;

        Pointer<Light> m_light;
    };

    class GuiSandboxLayer : public ImGuiLayer
    {
    public:
        explicit inline GuiSandboxLayer() : ImGuiLayer("Gui Sandbox Layer")
        {
            m_viewport01.SetFramebuffer(s_framebuffer01);
        }

        inline void OnImGuiRender() override
        {
            static bool doOnce = false;
            if (doOnce)
            {
                m_materialEditor.SetEntity(s_entity);
                doOnce = false;
            }

            m_docker.Render();
            m_viewport01.Render();
            m_stats.Render();
            m_scene.Render();
            m_materialEditor.Render();
            // ImGui::ShowDemoWindow();
        }

        inline void OnEvent(Event& e) override
        {
            m_docker.OnEvent(e);
            m_viewport01.OnEvent(e);
            m_stats.OnEvent(e);
            m_scene.OnEvent(e);
            m_materialEditor.OnEvent(e);
        }

    private:
        DockerWidget m_docker = DockerWidget("Docker Widget");
        ViewportWidget m_viewport01 = ViewportWidget("Viewport-01");
        StatsWidget m_stats = StatsWidget("Stats Widget");
        SceneWidget m_scene = SceneWidget("Scene Widget");
        MaterialEditorWidget m_materialEditor = MaterialEditorWidget("Material Editor");

    };
}
