#include "Sandbox.h"

#include <Alpha/Engine/Light/DirectionalLight.h>

namespace Alpha
{
    void SandboxLayer::Init()
    {
		m_scene = NewPointer<Scene>();
		GlobalStorage::AddScene("Scene_01", m_scene);
		m_scene->PushLight(NewPointer<DirectionalLight>());
		m_scene->SetFramebuffer(Framebuffer::Create(500, 500));
		m_scene->SetCamera(NewPointer<EulerCamera>());

		m_shader = Shader::Create("Physical-Based-Rendering", {
                {Shader::GLSL_VERTEX_SHADER, PROJECT_SOURCE_DIR + "Shaders/Forward.vs.glsl"},
                {Shader::GLSL_FRAGMENT_SHADER, PROJECT_SOURCE_DIR + "Shaders/Forward.fs.glsl"}
        });
		GlobalStorage::AddShader("Forward", m_shader);

		GlobalStorage::AddTexture2D("Brick", Texture2D::Create(PROJECT_SOURCE_DIR + "Assets/Brick.jpg"));

        Pointer<Material> defaultMaterial = NewPointer<Material>("DefaultMaterial");
        Pointer<Material> redMaterial = NewPointer<Material>("RedMaterial");
        Pointer<Material> brickMaterial = NewPointer<Material>("BrickMaterial");

        redMaterial->SetKd(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
        brickMaterial->AddTexture(ETextureType::TX_Diffuse, GlobalStorage::GetTexture2D("Brick"));

		GlobalStorage::AddMaterial("Default", defaultMaterial);
		GlobalStorage::AddMaterial("Red", redMaterial);
		GlobalStorage::AddMaterial("Brick", brickMaterial);

        Pointer<StaticMeshModel> cubeModel = NewPointer<StaticMeshModel>();
		cubeModel->Load(PROJECT_SOURCE_DIR + "Assets/Cube.fbx");
		GlobalStorage::AddStaticMeshModel("Cube", cubeModel);

		Pointer<StaticMeshModel> stanfordDragonModel = NewPointer<StaticMeshModel>();
		stanfordDragonModel->Load(PROJECT_SOURCE_DIR + "Assets/StanfordDragon.fbx");
		GlobalStorage::AddStaticMeshModel("StanfordDragon", stanfordDragonModel);

        m_stanfordDragonInstance = NewPointer<StaticMeshEntity>("Dragon", stanfordDragonModel);
        m_stanfordDragonInstance->SetMaterial(0, defaultMaterial);
        m_stanfordDragonInstance->SetMaterial(1, brickMaterial);
        m_stanfordDragonInstance->SetWorldLocation({0, -1, -2});
        m_stanfordDragonInstance->SetWorldScale({0.05, 0.05, 0.05});

		m_scene->PushComponent(m_stanfordDragonInstance);

        InitBSplineExample();
        InitTensorProductExample();

		LogTips();
    }

    void SandboxLayer::OnUpdate()
    {
		auto camera = Cast<EulerCamera>(m_scene->GetCamera());

        // AlphaEngine supports only US keyboard binding (aka QWERTY)
        if (Input::IsKeyPressed(ALPHA_KEY_W)) camera->MoveForward(1);
        if (Input::IsKeyPressed(ALPHA_KEY_S)) camera->MoveForward(-1);
        if (Input::IsKeyPressed(ALPHA_KEY_A)) camera->MoveRight(-1);
        if (Input::IsKeyPressed(ALPHA_KEY_D)) camera->MoveRight(1);

        if (Input::IsKeyPressed(ALPHA_KEY_C)) camera->SetZoom(ZOOM);
        if (Input::IsKeyPressed(ALPHA_KEY_P)) camera->SetZoom(camera->GetZoom() + 0.1f);
        if (Input::IsKeyPressed(ALPHA_KEY_M)) camera->SetZoom(camera->GetZoom() - 0.1f);

#ifdef PLATFORM_APPLE
		if (Input::IsMouseButtonPressed(ALPHA_MOUSE_BUTTON_2)) camera->Look(Input::GetMousePosition());
#else
		if (Input::IsMouseButtonPressed(ALPHA_MOUSE_BUTTON_3)) camera->Look(Input::GetMousePosition());
		if (Input::IsMouseButtonPressed(ALPHA_MOUSE_BUTTON_4)) camera->SetZoom(camera->GetZoom() + 0.1f);
		if (Input::IsMouseButtonPressed(ALPHA_MOUSE_BUTTON_5)) camera->SetZoom(camera->GetZoom() - 0.1f);
#endif


        ALPHA_ASSERT(m_scene->GetFramebuffer(), "Invalid Framebuffer");

		m_scene->Bind();
        m_shader->Bind();

        Renderer::Clear();
        Renderer::SetClearColor({0.2f, 0.3f, 0.3f, 1.0f});

		uint32 nLights = m_scene->GetLights().size();
		m_shader->SetUniform("nLights", static_cast<int32>(nLights));
		for (uint32 i = 0; i < nLights; ++i)
		{
			Pointer<Light> light = m_scene->GetLights()[i];
			std::string uniformName = "lights[" + ToString(i) + "]";
			m_shader->SetUniform(uniformName, light);
		}

		float aspectRatio = m_scene->GetFramebuffer()->GetAspectRatio();
        Matrix4x4 projectionMatrix = MakeProjectionMatrix(camera->GetZoom(), aspectRatio);
        Matrix4x4 viewMatrix = MakeViewMatrix(camera->GetWorldLocation(), camera->GetWorldRotation());
        TransformMatrix transformMatrix = {Matrix4x4(1), viewMatrix, projectionMatrix};

        m_stanfordDragonInstance->Draw(m_shader, transformMatrix);
        m_splineLineEntity->Draw(m_shader, transformMatrix);
        m_splinePointsEntity->Draw(m_shader, transformMatrix);
        m_tensorPointsEntity->Draw(m_shader, transformMatrix);
        m_tensorMeshEntity->Draw(m_shader, transformMatrix);

        m_shader->Unbind();
        m_scene->Unbind();
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
		for (uint32 i = 0; i < m_spline.GetNbPoints() - 1; ++i)
		{
			indicesPoints.emplace_back(i);
			indicesPoints.emplace_back(i + 1);
		}

        Pointer<StaticMeshModel> splinePointsModel = NewPointer<StaticMeshModel>();
		splinePointsModel->Load(verticesPoints, indicesPoints);
        m_splinePointsEntity = NewPointer<StaticMeshEntity>("Spline Entity", splinePointsModel);
        m_splinePointsEntity->SetMaterial(0, samplesMaterial);
        m_splinePointsEntity->SetDrawMode(EDrawMode::Lines);

        for (auto & sample : samples) verticesCurve.emplace_back(sample);

        for (uint32 i = 0; i < samples.size() - 1; ++i)
        {
            indicesCurve.emplace_back(i);
            indicesCurve.emplace_back(i + 1);
        }

        Pointer<StaticMeshModel> splineLineModel = NewPointer<StaticMeshModel>();
		splineLineModel->Load(verticesCurve, indicesCurve);
        m_splineLineEntity = NewPointer<StaticMeshEntity>("Spline Entity", splineLineModel);
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

        Pointer<StaticMeshModel> tensorPointsModel = NewPointer<StaticMeshModel>();
		tensorPointsModel->Load(verticesPoints, indicesPoints);
        m_tensorPointsEntity = NewPointer<StaticMeshEntity>("Tensor Point Entity", tensorPointsModel);
        m_tensorPointsEntity->SetMaterial(0, nodesMaterial);
        m_tensorPointsEntity->SetDrawMode(EDrawMode::Points);

        std::vector<uint32> indicesCurve;
        std::vector<Vertex> verticesCurve;

        verticesCurve.reserve(samples.size());
        for (auto & sample : samples) verticesCurve.emplace_back(sample);
        for (uint32 i = 0; i < samples.size(); ++i) indicesCurve.emplace_back(i);

        Pointer<StaticMeshModel> tensorMeshModel = NewPointer<StaticMeshModel>();
		tensorMeshModel->Load(verticesCurve, indicesCurve);
        m_tensorMeshEntity = NewPointer<StaticMeshEntity>("Tensor Mesh Entity", tensorMeshModel);
        m_tensorMeshEntity->SetMaterial(0, samplesMaterial);
        m_tensorMeshEntity->SetDrawMode(EDrawMode::Points);
    }

	void SandboxLayer::LogTips()
	{
		Logger::Info("(Tips) To move forward: Press W (Qwerty keyboard)");
		Logger::Info("(Tips) To move backward: Press S (Qwerty keyboard)");
		Logger::Info("(Tips) To move left: Press A (Qwerty keyboard)");
		Logger::Info("(Tips) To move right: Press D (Qwerty keyboard)");
		Logger::Info("(Tips) To zoom in: Press P (Qwerty keyboard)");
		Logger::Info("(Tips) To zoom out: Press M (Qwerty keyboard)");
		Logger::Info("(Tips) To reset the zoom: Press C (Qwerty keyboard)");
	}

    GuiSandboxLayer::GuiSandboxLayer() : ImGuiLayer("Gui Sandbox Layer")
    {
		Pointer<Framebuffer> fb = GlobalStorage::GetScene("Scene_01")->GetFramebuffer();
        m_viewportWidget01.SetFramebuffer(fb);
    }

    void GuiSandboxLayer::OnImGuiRender()
    {
		Pointer<Scene> scene = GlobalStorage::GetScene("Scene_01");
		m_sceneWidget.SetScene(scene);

		if (m_sceneWidget.IsSelectedEntityValid())
		{
			int32 index = m_sceneWidget.GetSelectedEntityIndex();
			Pointer<SceneComponent> component = scene->GetComponentAt(index);

			auto entity = Cast<StaticMeshEntity>(component);
			if (entity != m_materialEditor.GetEntity()) m_materialEditor.SetEntity(entity);
		} 
		else m_materialEditor.Clear();

        m_dockerWidget.Render();
        m_viewportWidget01.Render();
        m_statWidget.Render();
        m_sceneWidget.Render();
        m_materialEditor.Render();
        // ImGui::ShowDemoWindow();
    }

    void GuiSandboxLayer::OnEvent(Event &e)
    {
        m_dockerWidget.OnEvent(e);
        m_viewportWidget01.OnEvent(e);
        m_statWidget.OnEvent(e);
        m_sceneWidget.OnEvent(e);
        m_materialEditor.OnEvent(e);
    }
}