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

#include <Alpha/Engine/Renderer/OpenGL/OpenGLRenderer.h>


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
            m_pbrShader = Shader::Create("Physical-Based-Rendering", {
                {Shader::GLSL_VERTEX_SHADER, PROJECT_SOURCE_DIR + "Shaders/Pbr.vs.glsl"},
                {Shader::GLSL_FRAGMENT_SHADER, PROJECT_SOURCE_DIR + "Shaders/Pbr.fs.glsl"}
            });

            s_framebuffer01 = Framebuffer::Create(500, 500);

            m_directionalLight = NewPointer<DirectionalLight>();

            m_brickTexture = Texture2D::Create(PROJECT_SOURCE_DIR + "Assets/Brick.jpg");

            Pointer<Material> defaultMaterial = NewPointer<Material>("DefaultMaterial");
            Pointer<Material> redMaterial = NewPointer<Material>("RedMaterial");
            Pointer<Material> brickMaterial = NewPointer<Material>("BrickMaterial");

            defaultMaterial->SetKd(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
            redMaterial->SetKd(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
            brickMaterial->AddTexture(ETextureType::TX_Diffuse, m_brickTexture);

            m_cube = NewPointer<StaticMeshModel>();
            m_cube->Load(PROJECT_SOURCE_DIR + "Assets/Cube.fbx");

            m_stanfordDragon = NewPointer<StaticMeshModel>();
            m_stanfordDragon->Load(PROJECT_SOURCE_DIR + "Assets/StanfordDragon.fbx");

            m_stanfordDragonInstance = NewPointer<StaticMeshEntity>("Stanford Dragon 01", m_stanfordDragon);
            s_entity = m_stanfordDragonInstance;

            m_stanfordDragonInstance->SetMaterial(0, defaultMaterial);
            m_stanfordDragonInstance->SetMaterial(1, brickMaterial);

            m_stanfordDragonInstance->SetWorldLocation({0, -1, -2});
            m_stanfordDragonInstance->SetWorldScale({0.05, 0.05, 0.05});

            Logger::Info("(Tips) To move forward: Press W (Qwerty keyboard)");
            Logger::Info("(Tips) To move backward: Press S (Qwerty keyboard)");
            Logger::Info("(Tips) To move left: Press A (Qwerty keyboard)");
            Logger::Info("(Tips) To move right: Press D (Qwerty keyboard)");
            Logger::Info("(Tips) To zoom in: Press P (Qwerty keyboard)");
            Logger::Info("(Tips) To zoom out: Press M (Qwerty keyboard)");
            Logger::Info("(Tips) To reset the zoom: Press C (Qwerty keyboard)");

            m_spline.SetDegree(3);
            m_spline.SetNbPoints(16);
            m_spline.ResetKnotsVector();
            for (uint32 i = 0; i < m_spline.GetNbPoints(); ++i)
            {
                Vector3 point = Vector3(float(i), std::cosf(float(i * 2) / float(PI)) * float(HALF_PI), 0);
                m_spline.SetPointAt(i, point);
            }

            for (uint32 i = 0; i < m_spline.GetNbPoints(); ++i)
            {
                const std::string name = "Spline Node " + ToString(i);
                Pointer<StaticMeshEntity> node = NewPointer<StaticMeshEntity>(name, m_cube);

                node->SetMaterial(0, redMaterial);
                node->SetMaterial(1, redMaterial);

                node->SetWorldScale(Vector3(0.1f));
                node->SetWorldLocation(m_spline.GetPointAt(i));

                m_splineControlPoints.push_back(node);
            }

            std::vector<Vector3> samples = m_spline.Sample(0.25f);

            for (uint32 i = 0; i < samples.size(); ++i)
            {
                const std::string name = "Spline Node " + ToString(i);
                Pointer<StaticMeshEntity> node = NewPointer<StaticMeshEntity>(name, m_cube);

                node->SetMaterial(0, defaultMaterial);
                node->SetMaterial(1, defaultMaterial);

                node->SetWorldScale(Vector3(0.1f));
                node->SetWorldLocation(samples[i]);

                m_splineNodes.push_back(node);
            }

            /*
            Logger::Debug("Degree: {0}", m_spline.GetDegree());
            Logger::Debug("Order: {0}", m_spline.GetOrder());
            Logger::Debug("Nb Points: {0}", m_spline.GetNbPoints());
            Logger::Debug("Nb Knots: {0}", m_spline.GetNbKnots());

            Logger::Debug("Domain of def: x={0}, y={1}", m_spline.GetDomainOfDefinition().x, m_spline.GetDomainOfDefinition().y);

            for (uint32 i = 0; i < m_spline.GetNbPoints(); ++i)
            {
                Vector3 p = m_spline.GetPointAt(i);
                Logger::Debug("Spline point at {0}: x={1}, y={2}, z={3}", i, p.x, p.y, p.z);
            }

            for (uint32 i = 0; i < m_spline.GetNbKnots(); ++i)
            {
                float knot = m_spline.GetKnotAt(i);
                Logger::Debug("Spline knot at {0}: scalar={1}", i, knot);
            }
            */
        }

        inline void OnUpdate() override
        {
            // AlphaEngine supports only US keyboard binding (aka QWERTY)
            if (Input::IsKeyPressed(ALPHA_KEY_W)) m_camera.MoveForward(1);
            if (Input::IsKeyPressed(ALPHA_KEY_S)) m_camera.MoveForward(-1);
            if (Input::IsKeyPressed(ALPHA_KEY_A)) m_camera.MoveRight(-1);
            if (Input::IsKeyPressed(ALPHA_KEY_D)) m_camera.MoveRight(1);

            if (Input::IsKeyPressed(ALPHA_KEY_C)) m_camera.SetZoom(ZOOM);
            if (Input::IsKeyPressed(ALPHA_KEY_P)) m_camera.SetZoom(m_camera.GetZoom() + 0.1f);
            if (Input::IsKeyPressed(ALPHA_KEY_M)) m_camera.SetZoom(m_camera.GetZoom() - 0.1f);

            if (Input::IsMouseButtonPressed(ALPHA_MOUSE_BUTTON_3)) m_camera.Look(Input::GetMousePosition());
            if (Input::IsMouseButtonPressed(ALPHA_MOUSE_BUTTON_4)) m_camera.SetZoom(m_camera.GetZoom() + 0.1f);
            if (Input::IsMouseButtonPressed(ALPHA_MOUSE_BUTTON_5)) m_camera.SetZoom(m_camera.GetZoom() - 0.1f);

            ALPHA_ASSERT(s_framebuffer01, "Invalid Framebuffer: 01");

            s_framebuffer01->Bind();
            m_pbrShader->Bind();

            Renderer::Clear();
            Renderer::SetClearColor({0.2f, 0.3f, 0.3f, 1.0f});

            m_pbrShader->SetUniform("nLights", 1);
            m_pbrShader->SetUniform("lights[0]", m_directionalLight);

            float fb01AspectRatio = (float)s_framebuffer01->GetWidth() / (float)s_framebuffer01->GetHeight();
            Matrix4x4 projectionMatrix = MakeProjectionMatrix(m_camera.GetZoom(), fb01AspectRatio);

            Matrix4x4 viewMatrix = MakeViewMatrix(m_camera.GetWorldLocation(),  m_camera.GetWorldRotation());

            TransformMatrix transformMatrix = {Matrix4x4(1), viewMatrix, projectionMatrix};

            m_stanfordDragonInstance->Draw(m_pbrShader, transformMatrix);

            for (auto& splineNode : m_splineNodes) splineNode->Draw(m_pbrShader, transformMatrix);
            for (auto& controlPoint : m_splineControlPoints) controlPoint->Draw(m_pbrShader, transformMatrix);


            m_pbrShader->Unbind();
            s_framebuffer01->Unbind();
        }

    private:
        EulerCamera m_camera;
        Pointer<Shader> m_pbrShader;

        Pointer<Texture2D> m_brickTexture;

        Pointer<StaticMeshModel> m_cube;
        Pointer<StaticMeshModel> m_stanfordDragon;

        Pointer<StaticMeshEntity> m_stanfordDragonInstance;

        Pointer<Light> m_directionalLight;

        BSpline m_spline;
        std::vector<Pointer<StaticMeshEntity>> m_splineNodes;
        std::vector<Pointer<StaticMeshEntity>> m_splineControlPoints;

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
