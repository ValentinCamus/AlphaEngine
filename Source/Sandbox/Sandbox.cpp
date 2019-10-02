#include "Sandbox.h"

#include <Alpha/Engine/Light/DirectionalLight.h>

namespace Alpha
{
    static Pointer<Framebuffer> s_framebuffer01 = nullptr;

    void SandboxLayer::Init()
    {
        m_pbrShader = Shader::Create("Physical-Based-Rendering", {
                {Shader::GLSL_VERTEX_SHADER, PROJECT_SOURCE_DIR + "Shaders/Forward.vs.glsl"},
                {Shader::GLSL_FRAGMENT_SHADER, PROJECT_SOURCE_DIR + "Shaders/Forward.fs.glsl"}
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

#ifdef PLATFORM_APPLE
		if (Input::IsMouseButtonPressed(ALPHA_MOUSE_BUTTON_2)) m_camera.Look(Input::GetMousePosition());
#else
		if (Input::IsMouseButtonPressed(ALPHA_MOUSE_BUTTON_3)) m_camera.Look(Input::GetMousePosition());
		if (Input::IsMouseButtonPressed(ALPHA_MOUSE_BUTTON_4)) m_camera.SetZoom(m_camera.GetZoom() + 0.1f);
		if (Input::IsMouseButtonPressed(ALPHA_MOUSE_BUTTON_5)) m_camera.SetZoom(m_camera.GetZoom() - 0.1f);
#endif


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

        m_tensorPointsEntity->Draw(m_pbrShader, transformMatrix);
        m_tensorMeshEntity->Draw(m_pbrShader, transformMatrix);

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

        for (uint32 i = 0; i < m_spline.GetNbPoints(); ++i) verticesPoints.emplace_back(m_spline.GetPointAt(i));
        for (uint32 i = 0; i < m_spline.GetNbPoints(); ++i) indicesPoints.emplace_back(i);

        m_splinePointsModel = NewPointer<StaticMeshModel>();
        m_splinePointsModel->Load(verticesPoints, indicesPoints);

        m_splinePointsEntity = NewPointer<StaticMeshEntity>("Spline Entity", m_splinePointsModel);
        m_splinePointsEntity->SetMaterial(0, samplesMaterial);
        m_splinePointsEntity->SetDrawMode(EDrawMode::Points);

        for (auto & sample : samples) verticesCurve.emplace_back(sample);

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

        for (uint32 i = 0; i < m_tensorProduct.GetSurface().width; ++i)
            for (uint32 j = 0; j < m_tensorProduct.GetSurface().height; ++j)
                m_tensorProduct.SetPointAt(i, j, {i, Random::GetFloat(-1, 1), j});

        std::vector<Vector3> samples = m_tensorProduct.GetSamples(0.1f, 0.1f);

        Pointer<Material> samplesMaterial = NewPointer<Material>("TensorProductSamples");
        Pointer<Material> nodesMaterial = NewPointer<Material>("TensorProductNodes");

        samplesMaterial->SetKd(Vector4(0.0f, 0.0f, 1.0f, 1.0f));
        nodesMaterial->SetKd(Vector4(1.0f, 1.0f, 0.0f, 1.0f));

        std::vector<uint32> indicesPoints;
        std::vector<Vertex> verticesPoints;

        for (uint32 i = 0; i < m_tensorProduct.GetSurface().width; ++i)
        {
            for (uint32 j = 0; j < m_tensorProduct.GetSurface().height; ++j)
            {
                verticesPoints.emplace_back(m_tensorProduct.GetPointAt(i, j));
                indicesPoints.emplace_back(i + j * m_tensorProduct.GetSurface().width);
            }
        }

        m_tensorPointsModel = NewPointer<StaticMeshModel>();
        m_tensorPointsModel->Load(verticesPoints, indicesPoints);

        m_tensorPointsEntity = NewPointer<StaticMeshEntity>("Tensor Point Entity", m_tensorPointsModel);
        m_tensorPointsEntity->SetMaterial(0, nodesMaterial);
        m_tensorPointsEntity->SetDrawMode(EDrawMode::Points);

        std::vector<uint32> indicesCurve;
        std::vector<Vertex> verticesCurve;

        verticesCurve.reserve(samples.size());
        for (auto & sample : samples) verticesCurve.emplace_back(sample);
        for (uint32 i = 0; i < samples.size(); ++i) indicesCurve.emplace_back(i);

        m_tensorMeshModel = NewPointer<StaticMeshModel>();
        m_tensorMeshModel->Load(verticesCurve, indicesCurve);

        m_tensorMeshEntity = NewPointer<StaticMeshEntity>("Tensor Mesh Entity", m_tensorMeshModel);
        m_tensorMeshEntity->SetMaterial(0, samplesMaterial);
        m_tensorMeshEntity->SetDrawMode(EDrawMode::Points);
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