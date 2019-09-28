#include "Sandbox.h"

#include <Alpha/Engine/Light/DirectionalLight.h>

namespace Alpha
{
    static Pointer<Framebuffer> s_framebuffer01 = nullptr;

    void SandboxLayer::Init()
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

        redMaterial->SetKd(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
        brickMaterial->AddTexture(ETextureType::TX_Diffuse, m_brickTexture);

        m_cube = NewPointer<StaticMeshModel>();
        m_cube->Load(PROJECT_SOURCE_DIR + "Assets/Cube.fbx");

        m_stanfordDragon = NewPointer<StaticMeshModel>();
        m_stanfordDragon->Load(PROJECT_SOURCE_DIR + "Assets/StanfordDragon.fbx");

        m_stanfordDragonInstance = NewPointer<StaticMeshEntity>("Dragon", m_stanfordDragon);
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

        InitBSplineExample();
        InitTensorProductExample();
    }

    void SandboxLayer::OnUpdate()
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

        m_splineLineEntity->Draw(m_pbrShader, transformMatrix);
        m_splinePointsEntity->Draw(m_pbrShader, transformMatrix);

        /*
        for (auto& tensorNode : m_tensorProductNodes)
            tensorNode->Draw(m_pbrShader, transformMatrix);
        for (auto& tensorControlPoint : m_tensorProductControlPoints)
            tensorControlPoint->Draw(m_pbrShader, transformMatrix);
        */

        m_pbrShader->Unbind();
        s_framebuffer01->Unbind();
    }

    void SandboxLayer::InitBSplineExample()
    {
        std::vector<Vertex> verticesPoints, verticesCurve;
        std::vector<uint32> indicesPoints, indicesCurve;

        Pointer<Material> samplesMaterial = NewPointer<Material>("BSplineSamples");
        Pointer<Material> nodesMaterial = NewPointer<Material>("BSplineNodes");
        nodesMaterial->SetKd(Vector4(1.0f, 0.0f, 0.0f, 1.0f));

        m_spline.SetDegree(3);
        m_spline.SetNbPoints(16);
        m_spline.ResetKnotsVector();
        for (uint32 i = 0; i < m_spline.GetNbPoints(); ++i)
        {
            float xPos = i - m_spline.GetNbPoints() / 2.0f;
            float yPos = Random::GetFloat(-float(PI), float(PI));
            float zPos = Random::GetFloat(-6, -4);
            Vector3 point = Vector3(xPos, yPos, zPos);
            m_spline.SetPointAt(i, point);
        }

        std::vector<Vector3> samples = m_spline.GetSamples(0.1f);

        for (uint32 i = 0; i < m_spline.GetNbPoints(); ++i)
            verticesPoints.emplace_back(m_spline.GetPointAt(i), Vector(1), Vector2(0));

        for (uint32 i = 0; i < m_spline.GetNbPoints() - 1; ++i) indicesPoints.emplace_back(i);

        m_splinePointsModel = NewPointer<StaticMeshModel>();
        m_splinePointsModel->Load(verticesPoints, indicesPoints);

        m_splinePointsEntity = NewPointer<StaticMeshEntity>("Spline Entity", m_splinePointsModel);
        m_splinePointsEntity->SetMaterial(0, samplesMaterial);
        m_splinePointsEntity->SetDrawMode(EDrawMode::Points);

        for (auto & sample : samples)
            verticesCurve.emplace_back(sample, Vector(1), Vector2(0));

        for (uint32 i = 0; i < samples.size() - 1; ++i)
        {
            indicesCurve.emplace_back(i);
            indicesCurve.emplace_back(i + 1);
        }

        m_splineLineModel = NewPointer<StaticMeshModel>();
        m_splineLineModel->Load(verticesCurve, indicesCurve);

        m_splineLineEntity = NewPointer<StaticMeshEntity>("Spline Entity", m_splineLineModel);
        m_splineLineEntity->SetMaterial(0, nodesMaterial);
        m_splineLineEntity->SetDrawMode(EDrawMode::Lines);
    }

    void SandboxLayer::InitTensorProductExample()
    {
        m_tensorProduct.SetDegree(3);
        m_tensorProduct.Resize(10, 10);

        for (uint32 i = 0; i < 10; ++i)
        {
            for (uint32 j = 0; j < 10; ++j)
            {
                m_tensorProduct.SetPointAt(i, j, Vector(float(i), Random::GetFloat(-1, 1), float(j)));
            }
        }

        Vector2 xDef = m_tensorProduct.GetValidRangeWidth();
        Vector2 yDef = m_tensorProduct.GetValidRangeHeight();

        std::vector<Vector3> samples;

        for (float u = xDef.x; u < xDef.y; u += 0.5f)
        {
            for (float v = yDef.x; v < yDef.y; v += 0.5f)
            {
                samples.push_back(m_tensorProduct.Evaluate(u, v));
            }
        }

        Pointer<Material> samplesMaterial = NewPointer<Material>("TensorProductSamples");
        Pointer<Material> nodesMaterial = NewPointer<Material>("TensorProductNodes");

        samplesMaterial->SetKd(Vector4(0.0f, 0.0f, 1.0f, 1.0f));
        nodesMaterial->SetKd(Vector4(1.0f, 1.0f, 0.0f, 1.0f));

        for (uint32 i = 0; i < m_tensorProduct.GetSurface().width; ++i)
        {
            for (uint32 j = 0; j < m_tensorProduct.GetSurface().height; ++j)
            {
                const std::string name = "TensorProduct Node " + ToString(i) + ToString(j);
                Pointer<StaticMeshEntity> node = NewPointer<StaticMeshEntity>(name, m_cube);

                node->SetMaterial(0, nodesMaterial);
                node->SetMaterial(1, nodesMaterial);

                node->SetWorldScale(Vector3(0.1f));
                node->SetWorldLocation(m_tensorProduct.GetPointAt(i, j));

                m_tensorProductControlPoints.push_back(node);
            }
        }

        for (uint32 i = 0; i < samples.size(); ++i)
        {
            const std::string name = "TensorProduct Sample " + ToString(i);
            Pointer<StaticMeshEntity> node = NewPointer<StaticMeshEntity>(name, m_cube);

            node->SetMaterial(0, samplesMaterial);
            node->SetMaterial(1, samplesMaterial);

            node->SetWorldScale(Vector3(0.1f));
            node->SetWorldLocation(samples[i]);

            m_tensorProductNodes.push_back(node);
        }
    }

    GuiSandboxLayer::GuiSandboxLayer() : ImGuiLayer("Gui Sandbox Layer")
    {
        m_viewport01.SetFramebuffer(s_framebuffer01);
    }

    void GuiSandboxLayer::OnImGuiRender()
    {
        m_docker.Render();
        m_viewport01.Render();
        m_stats.Render();
        m_scene.Render();
        m_materialEditor.Render();
        // ImGui::ShowDemoWindow();
    }

    void GuiSandboxLayer::OnEvent(Event &e)
    {
        m_docker.OnEvent(e);
        m_viewport01.OnEvent(e);
        m_stats.OnEvent(e);
        m_scene.OnEvent(e);
        m_materialEditor.OnEvent(e);
    }
}