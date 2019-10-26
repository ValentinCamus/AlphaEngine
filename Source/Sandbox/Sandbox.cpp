#include "Sandbox.h"

#include <Alpha/Engine/Light/DirectionalLight.h>

namespace Alpha
{
    void SandboxLayer::Init()
    {
        Pointer<EulerCamera> camera = NewPointer<EulerCamera>();

		m_scene = NewPointer<Scene>();
		m_scene->PushLight(NewPointer<DirectionalLight>());
		m_scene->SetFramebuffer(Framebuffer::Create(1920, 1080));
		m_scene->SetCamera(camera);
		m_shader = Shader::Create("Forward", {
                {Shader::GLSL_VERTEX_SHADER, PROJECT_SOURCE_DIR + "Shaders/Forward.vs.glsl"},
                {Shader::GLSL_FRAGMENT_SHADER, PROJECT_SOURCE_DIR + "Shaders/Forward.fs.glsl"}
        });
        GlobalStorage::AddScene("Scene_01", m_scene);
		GlobalStorage::AddShader("Forward", m_shader);

        m_depthScene = NewPointer<Scene>();
        m_depthScene->SetFramebuffer(DepthBuffer::Create(1920, 1080));
        m_depthScene->SetCamera(camera);
        m_depthShader = Shader::Create("Depth", {
                {Shader::GLSL_VERTEX_SHADER, PROJECT_SOURCE_DIR + "Shaders/Depth.vs.glsl"},
                {Shader::GLSL_FRAGMENT_SHADER, PROJECT_SOURCE_DIR + "Shaders/Depth.fs.glsl"}
        });
        m_debugDepthShader = Shader::Create("DebugDepth", {
                {Shader::GLSL_VERTEX_SHADER, PROJECT_SOURCE_DIR + "Shaders/DebugDepth.vs.glsl"},
                {Shader::GLSL_FRAGMENT_SHADER, PROJECT_SOURCE_DIR + "Shaders/DebugDepth.fs.glsl"}
        });
        GlobalStorage::AddScene("DepthScene_01", m_depthScene);
        GlobalStorage::AddShader("Depth", m_depthShader);
        GlobalStorage::AddShader("Depth", m_debugDepthShader);

        Pointer<Texture2D> brickTexture = Texture2D::Create(PROJECT_SOURCE_DIR + "Assets/Brick.jpg");
		GlobalStorage::AddTexture2D("Brick", brickTexture);

        Pointer<Material> defaultMaterial = NewPointer<Material>("DefaultMaterial");
        Pointer<Material> redMaterial = NewPointer<Material>("RedMaterial");
        Pointer<Material> brickMaterial = NewPointer<Material>("BrickMaterial");

        redMaterial->SetKd(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
        brickMaterial->AddTexture(ETextureType::TX_Diffuse, brickTexture);

		GlobalStorage::AddMaterial("Default", defaultMaterial);
		GlobalStorage::AddMaterial("Red", redMaterial);
		GlobalStorage::AddMaterial("Brick", brickMaterial);

        Pointer<StaticMeshModel> cubeModel = NewPointer<StaticMeshModel>();
		cubeModel->Load(PROJECT_SOURCE_DIR + "Assets/Cube.fbx");
		GlobalStorage::AddStaticMeshModel("Cube", cubeModel);

		Pointer<StaticMeshModel> stanfordDragonModel = NewPointer<StaticMeshModel>();
		stanfordDragonModel->Load(PROJECT_SOURCE_DIR + "Assets/StanfordDragon.fbx");
		GlobalStorage::AddStaticMeshModel("StanfordDragon", stanfordDragonModel);

        Pointer<StaticMeshModel> tileModel = NewPointer<StaticMeshModel>();
        tileModel->Load(PROJECT_SOURCE_DIR + "Assets/Tile.fbx");
        GlobalStorage::AddStaticMeshModel("Tile", tileModel);

        m_tileInstance = NewPointer<StaticMeshEntity>("Tile", tileModel);
        m_tileInstance->SetMaterial(0, defaultMaterial);
        m_tileInstance->SetWorldLocation({-2, 0, -3});
        m_tileInstance->SetWorldRotation({0, 45, 0});

        m_cubeInstance = NewPointer<StaticMeshEntity>("Cube", cubeModel);
        m_cubeInstance->SetMaterial(0, defaultMaterial);
        m_cubeInstance->SetMaterial(1, brickMaterial);
        m_cubeInstance->SetWorldLocation({1, 0, -5});
        m_cubeInstance->SetWorldRotation({30, 25, 42});

        m_stanfordDragonInstance = NewPointer<StaticMeshEntity>("Dragon", stanfordDragonModel);
        m_stanfordDragonInstance->SetMaterial(0, defaultMaterial);
        m_stanfordDragonInstance->SetMaterial(1, brickMaterial);
        m_stanfordDragonInstance->SetWorldLocation({0, -1, -2});
        m_stanfordDragonInstance->SetWorldScale({0.05, 0.05, 0.05});

        m_scene->PushComponent(m_cubeInstance);
		m_scene->PushComponent(m_stanfordDragonInstance);

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

        if (Input::IsKeyPressed(ALPHA_KEY_Z)) camera->SetViewType(Camera::EViewType::VT_Perspective);
        if (Input::IsKeyPressed(ALPHA_KEY_K)) camera->SetViewType(Camera::EViewType::VT_Orthographic);

#ifdef PLATFORM_APPLE
		if (Input::IsMouseButtonPressed(ALPHA_MOUSE_BUTTON_2)) camera->Look(Input::GetMousePosition());
#else
		if (Input::IsMouseButtonPressed(ALPHA_MOUSE_BUTTON_3)) camera->Look(Input::GetMousePosition());
		if (Input::IsMouseButtonPressed(ALPHA_MOUSE_BUTTON_4)) camera->SetZoom(camera->GetZoom() + 0.1f);
		if (Input::IsMouseButtonPressed(ALPHA_MOUSE_BUTTON_5)) camera->SetZoom(camera->GetZoom() - 0.1f);
#endif
		RenderShadowMaps();

        ALPHA_ASSERT(m_scene->GetFramebuffer(), "Invalid Framebuffer");

		m_scene->Bind();
        m_shader->Bind();

        Renderer::SetClearColor({0.2f, 0.3f, 0.3f, 1.0f});
        Renderer::Clear();

		auto nLights = static_cast<uint32>(m_scene->GetLights().size());
		m_shader->SetUniform("nLights", static_cast<int32>(nLights));
		for (uint32 i = 0; i < nLights; ++i)
		{
			Pointer<Light> light = m_scene->GetLights()[i];
			std::string uniformName = "lights[" + ToString(i) + "]";
			m_shader->SetUniform(uniformName, light);
		}

        Matrix4x4 projectionMatrix = Matrix4x4(1.0f);
		float aspectRatio = m_scene->GetFramebuffer()->GetAspectRatio();

        switch (camera->GetViewType())
        {
            case Camera::EViewType::VT_Orthographic:
            {
                float left = -aspectRatio * camera->GetZoom();
                float right = aspectRatio * camera->GetZoom();
                float bottom = -camera->GetZoom();
                float top = camera->GetZoom();
                projectionMatrix = MakeOrthographicMatrix(left, right, bottom, top);
                break;
            }
            case Camera::EViewType::VT_Perspective:
            {
                float fovY = camera->GetZoom();
                projectionMatrix = MakePerspectiveMatrix(fovY, aspectRatio);
                break;
            }
        }

        Matrix4x4 viewMatrix = MakeViewMatrix(camera->GetWorldLocation(), camera->GetWorldRotation());
        TransformMatrix transformMatrix = {Matrix4x4(1), viewMatrix, projectionMatrix};

        m_tileInstance->Draw(m_shader, transformMatrix);
        m_cubeInstance->Draw(m_shader, transformMatrix);
        m_stanfordDragonInstance->Draw(m_shader, transformMatrix);

        m_shader->Unbind();

        m_scene->Unbind();
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

    void SandboxLayer::RenderShadowMaps()
    {
        ALPHA_ASSERT(m_depthScene->GetFramebuffer(), "Invalid Framebuffer");

        m_depthScene->Bind();
        m_depthShader->Bind();

        Renderer::SetClearColor({0.0f, 0.0f, 0.0f, 1.0f});
        Renderer::Clear();

        auto camera = Cast<EulerCamera>(m_depthScene->GetCamera());

        Matrix4x4 projectionMatrix = Matrix4x4(1.0f);
        float aspectRatio = m_depthScene->GetFramebuffer()->GetAspectRatio();

        switch (camera->GetViewType())
        {
            case Camera::EViewType::VT_Orthographic:
            {
                float left = -aspectRatio * camera->GetZoom();
                float right = aspectRatio * camera->GetZoom();
                float bottom = -camera->GetZoom();
                float top = camera->GetZoom();
                projectionMatrix = MakeOrthographicMatrix(left, right, bottom, top);
                break;
            }
            case Camera::EViewType::VT_Perspective:
            {
                float fovY = camera->GetZoom();
                projectionMatrix = MakePerspectiveMatrix(fovY, aspectRatio);
                break;
            }
        }

        Matrix4x4 viewMatrix = MakeViewMatrix(camera->GetWorldLocation(), camera->GetWorldRotation());
        TransformMatrix transformMatrix = {Matrix4x4(1), viewMatrix, projectionMatrix};

        Renderer::Enable(Renderer::EOption::DiscardMaterial);
        m_tileInstance->Draw(m_depthShader, transformMatrix);
        m_cubeInstance->Draw(m_depthShader, transformMatrix);
        m_stanfordDragonInstance->Draw(m_depthShader, transformMatrix);
        Renderer::Disable(Renderer::EOption::DiscardMaterial);

        m_depthShader->Unbind();
        m_depthScene->Unbind();
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
            auto index = static_cast<uint32>(m_sceneWidget.GetSelectedEntityIndex());
			auto entity = Cast<StaticMeshEntity>(scene->GetComponentAt(index));
			if (entity != m_materialEditor.GetEntity()) m_materialEditor.SetEntity(entity);
		}
		else m_materialEditor.Clear();

        m_dockerWidget.Render();
        m_viewportWidget01.Render();
        m_statWidget.Render();
        m_sceneWidget.Render();
        m_materialEditor.Render();
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
