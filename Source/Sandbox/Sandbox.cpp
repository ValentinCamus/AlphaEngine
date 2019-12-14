#include "Sandbox.h"

#include <Alpha/Engine/Light/DirectionalLight.h>

namespace Alpha
{
    void SandboxLayer::Init()
    {
        m_flatShader = Shader::Create("Flat", {
                {Shader::GLSL_VERTEX_SHADER, ALPHA_SHADERS_DIR + "Flat.vs.glsl"},
                {Shader::GLSL_FRAGMENT_SHADER, ALPHA_SHADERS_DIR + "Flat.fs.glsl"}
        });
        m_animForwardShader = Shader::Create("AnimForward", {
                {Shader::GLSL_VERTEX_SHADER, ALPHA_SHADERS_DIR + "AnimatedObject.vs.glsl"},
                {Shader::GLSL_FRAGMENT_SHADER, ALPHA_SHADERS_DIR + "Forward.fs.glsl"}
        });
        m_staticForwardShader = Shader::Create("StaticForward", {
                {Shader::GLSL_VERTEX_SHADER, ALPHA_SHADERS_DIR + "StaticObject.vs.glsl"},
                {Shader::GLSL_FRAGMENT_SHADER, ALPHA_SHADERS_DIR + "Forward.fs.glsl"}
        });
        m_skyboxShader = Shader::Create("Skybox", {
                {Shader::GLSL_VERTEX_SHADER, ALPHA_SHADERS_DIR + "Skybox.vs.glsl"},
                {Shader::GLSL_FRAGMENT_SHADER, ALPHA_SHADERS_DIR + "Skybox.fs.glsl"}
        });
        m_cubemapsShader = Shader::Create("Cubemaps", {
                {Shader::GLSL_VERTEX_SHADER, ALPHA_SHADERS_DIR + "Cubemaps.vs.glsl"},
                {Shader::GLSL_FRAGMENT_SHADER, ALPHA_SHADERS_DIR + "Cubemaps.fs.glsl"}
        });
        m_depthShader = Shader::Create("Depth", {
                {Shader::GLSL_VERTEX_SHADER, ALPHA_SHADERS_DIR + "Depth.vs.glsl"},
                {Shader::GLSL_FRAGMENT_SHADER, ALPHA_SHADERS_DIR + "Depth.fs.glsl"}
        });
        m_debugDepthShader = Shader::Create("DebugDepth", {
                {Shader::GLSL_VERTEX_SHADER, ALPHA_SHADERS_DIR + "DebugDepth.vs.glsl"},
                {Shader::GLSL_FRAGMENT_SHADER, ALPHA_SHADERS_DIR + "DebugDepth.fs.glsl"}
        });
        m_debugNormalShader = Shader::Create("DebugNormal", {
                {Shader::GLSL_VERTEX_SHADER, ALPHA_SHADERS_DIR + "DebugNormal.vs.glsl"},
                {Shader::GLSL_GEOMETRY_SHADER, ALPHA_SHADERS_DIR + "DebugNormal.gs.glsl"},
                {Shader::GLSL_FRAGMENT_SHADER, ALPHA_SHADERS_DIR + "DebugNormal.fs.glsl"}
        });

		m_scene = NewPointer<Scene>();
		m_scene->SetFramebuffer(Framebuffer::Create(32, 32));
		m_scene->SetCamera(NewPointer<EulerCamera>());

		Pointer<DirectionalLight> sunLight = NewPointer<DirectionalLight>();
        sunLight->SetWorldRotation({45.0f, 0.0f, 0.0f});
        sunLight->SetWorldLocation({-0.25f, 1.0f, 0.0f});
        sunLight->SetColor(Color4(1.0f, 1.0f, 0.9f, 1.0f));
        sunLight->CreateShadowMap(4096, 4096);

//        Pointer<PointLight> blueLight = NewPointer<PointLight>();
//        blueLight->SetColor(Color4(0.0f, 0.0f, 1.0f, 1.0f));
//        blueLight->SetWorldLocation({-0.4, 0, -2});
//        blueLight->SetAttenuation({1.0, 0.0014, 0.000007});
//        blueLight->CreateShadowMap(4096, 4096);
//
//        Pointer<PointLight> greenLight = NewPointer<PointLight>();
//        greenLight->SetColor(Color4(0.0f, 1.0f, 0.0f, 1.0f));
//        greenLight->SetWorldLocation({0.4, 0, -2});
//        greenLight->SetAttenuation({1.0, 0.0014, 0.000007});
//        greenLight->CreateShadowMap(4096, 4096);
//
//        Pointer<SpotLight> flashlight = NewPointer<SpotLight>(12.5 * DEG2RAD);
//        flashlight->SetWorldRotation({0.0f, -10.0f, 0.0f});
//        flashlight->SetWorldLocation({0, -0.75f, -0.5f});
//        flashlight->SetColor(Color4(1.0f, 1.0f, 0.9f, 1.0f));
//        flashlight->CreateShadowMap(4096, 4096);

        m_scene->PushLight(sunLight);
        // m_scene->PushLight(blueLight);
        // m_scene->PushLight(greenLight);
        // m_scene->PushLight(flashlight);

        Pointer<Texture2D> brickTexture = Texture2D::Create(ALPHA_ASSETS_DIR + "Brick.jpg");
        Pointer<Texture2D> stanLeeTexture = Texture2D::Create(ALPHA_ASSETS_DIR + "StanLee.png");

        Pointer<Material> defaultMaterial = NewPointer<Material>("DefaultMaterial");
        Pointer<Material> redMaterial = NewPointer<Material>("RedMaterial");
        Pointer<Material> greenMaterial = NewPointer<Material>("GreenMaterial");
        Pointer<Material> brickMaterial = NewPointer<Material>("BrickMaterial");
        Pointer<Material> lightMaterial = NewPointer<Material>("LightMaterial");
        Pointer<Material> stanLeeMaterial = NewPointer<Material>("LightMaterial");

        redMaterial->SetKd(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
        greenMaterial->SetKd(Vector4(0.0f, 1.0f, 0.0f, 1.0f));
        brickMaterial->AddTexture(ETextureType::TX_Diffuse, brickTexture);
        stanLeeMaterial->AddTexture(ETextureType::TX_Diffuse, stanLeeTexture);

        Pointer<StaticMeshModel> tileModel = StaticMeshModel::Create(ALPHA_ASSETS_DIR + "Tile.fbx");
        Pointer<StaticMeshModel> cubeModel = StaticMeshModel::Create(ALPHA_ASSETS_DIR + "Cube.fbx");
        Pointer<StaticMeshModel> sphereModel = StaticMeshModel::Create(ALPHA_ASSETS_DIR + "Sphere.fbx");
		Pointer<StaticMeshModel> bustModel = StaticMeshModel::Create(ALPHA_ASSETS_DIR + "Bust.fbx");
        Pointer<StaticMeshModel> StanLeeModel = StaticMeshModel::Create(ALPHA_ASSETS_DIR + "StanLee.obj");

        m_mirrorInstance = NewPointer<StaticMeshInstance>("Mirror", cubeModel);
        m_mirrorInstance->SetMaterial(0, defaultMaterial);
        m_mirrorInstance->SetWorldLocation({0.0f, 1.0f, -3.0f});
        m_mirrorInstance->SetWorldScale({0.1, 0.1, 0.1});

        m_cubeInstance = NewPointer<StaticMeshInstance>("Cube", cubeModel);
        m_cubeInstance->SetMaterial(0, defaultMaterial);
        m_cubeInstance->SetWorldLocation({0.088f, -1.120f, -2.611f});
        m_cubeInstance->SetWorldRotation({0, 0, 0});
        m_cubeInstance->SetWorldScale({10.0f, 0.05f, 10.0f});

        m_planeInstance = NewPointer<StaticMeshInstance>("Tile", tileModel);
        m_planeInstance->SetMaterial(0, brickMaterial);
        m_planeInstance->SetWorldLocation({0, -1, -1});
        m_planeInstance->SetWorldScale({0.1, 0.1, 0.1});

        m_lightInstance = NewPointer<StaticMeshInstance>("Light", cubeModel);
        m_lightInstance->SetMaterial(0, lightMaterial);
        m_lightInstance->SetWorldScale({0.1f, 0.1f, 0.1f});

        m_bustInstance = NewPointer<StaticMeshInstance>("Bust", bustModel);
        m_bustInstance->SetMaterial(0, defaultMaterial);
        m_bustInstance->SetWorldLocation({0, -1, -2});
        m_bustInstance->SetWorldScale({10.0f, 10.0f, 10.0f});
        m_bustInstance->SetWorldRotation({-90, 0, 0});

        m_stanLeeInstance = NewPointer<StaticMeshInstance>("StanLee", StanLeeModel);
        m_stanLeeInstance->SetMaterial(0, stanLeeMaterial);
        m_stanLeeInstance->SetMaterial(1, redMaterial);
        m_stanLeeInstance->SetWorldLocation({1, -1, -2});
        m_stanLeeInstance->SetWorldScale({0.008f, 0.008f, 0.008f});

        m_scene->PushComponent(m_cubeInstance);
		m_scene->PushComponent(m_bustInstance);
        m_scene->PushComponent(m_mirrorInstance);
        m_scene->PushComponent(m_stanLeeInstance);

        GlobalStorage::AddScene("Scene_01", m_scene);

        GlobalStorage::AddShader("Flat", m_flatShader);
        GlobalStorage::AddShader("Depth", m_depthShader);
        GlobalStorage::AddShader("Skybox", m_skyboxShader);
        GlobalStorage::AddShader("Forward", m_staticForwardShader);
        GlobalStorage::AddShader("Cubemaps", m_cubemapsShader);
        GlobalStorage::AddShader("DebugDepth", m_debugDepthShader);
        GlobalStorage::AddShader("DebugNormal", m_debugNormalShader);

        GlobalStorage::AddTexture2D("Brick", brickTexture);
        GlobalStorage::AddTexture2D("StanLee", stanLeeTexture);

        GlobalStorage::AddMaterial("Default", defaultMaterial);
        GlobalStorage::AddMaterial("Red", redMaterial);
        GlobalStorage::AddMaterial("Green", greenMaterial);
        GlobalStorage::AddMaterial("Brick", brickMaterial);
        GlobalStorage::AddMaterial("StanLee", stanLeeMaterial);
        GlobalStorage::AddMaterial("Light", lightMaterial);

        GlobalStorage::AddStaticMeshModel("Cube", cubeModel);
        GlobalStorage::AddStaticMeshModel("Tile", tileModel);
        GlobalStorage::AddStaticMeshModel("Sphere", sphereModel);
        GlobalStorage::AddStaticMeshModel("Dragon01", bustModel);
        GlobalStorage::AddStaticMeshModel("Dragon02", StanLeeModel);

        std::map<Skybox::EFaceOrientation, std::string> skyboxFaces = {
            {Skybox::Right, ALPHA_ASSETS_DIR + "Skybox/Right.jpg"},
            {Skybox::Left, ALPHA_ASSETS_DIR + "Skybox/Left.jpg"},
            {Skybox::Top, ALPHA_ASSETS_DIR + "Skybox/Top.jpg"},
            {Skybox::Bottom, ALPHA_ASSETS_DIR + "Skybox/Bottom.jpg"},
            {Skybox::Front, ALPHA_ASSETS_DIR + "Skybox/Front.jpg"},
            {Skybox::Back, ALPHA_ASSETS_DIR + "Skybox/Back.jpg"},
        };
        m_skybox = NewPointer<Skybox>(skyboxFaces);

        LogTips();
    }

    void SandboxLayer::OnUpdate()
    {
		auto camera = Cast<EulerCamera>(m_scene->GetCamera());

		CheckInputs();
		UpdateShadowMaps({m_bustInstance, m_stanLeeInstance, m_cubeInstance});

        m_scene->Bind();

        TransformMatrix transformMatrix = CalculateTransformMatrix();

        Renderer::SetClearColor({0.2f, 0.3f, 0.3f, 1.0f});
        Renderer::Clear();

        DrawSkybox(transformMatrix);

        for (const Pointer<Light>& light : m_scene->GetLights())
        {
            m_staticForwardShader->Bind();

            light->GetDepthBuffer()->GetTexture()->Bind(8);

            m_staticForwardShader->SetUniform("u_light", light);
            m_staticForwardShader->SetUniform("u_viewPosition", camera->GetWorldLocation());

            m_bustInstance->BindMaterials();
            m_bustInstance->Draw(m_staticForwardShader, &transformMatrix.projection, &transformMatrix.view);
            m_bustInstance->UnbindMaterials();

            m_stanLeeInstance->BindMaterials();
            m_stanLeeInstance->Draw(m_staticForwardShader, &transformMatrix.projection, &transformMatrix.view);
            m_stanLeeInstance->UnbindMaterials();

            m_cubeInstance->BindMaterials();
            m_cubeInstance->Draw(m_staticForwardShader, &transformMatrix.projection, &transformMatrix.view);
            m_cubeInstance->UnbindMaterials();

            light->GetDepthBuffer()->GetTexture()->Unbind();

            m_staticForwardShader->Unbind();

            m_flatShader->Bind();
            m_flatShader->SetUniform("u_color", light->GetColor());
            m_lightInstance->SetWorldLocation(light->GetWorldLocation());
            m_lightInstance->SetWorldRotation(light->GetWorldRotation());
            m_lightInstance->Draw(m_flatShader, &transformMatrix.projection, &transformMatrix.view);
            m_flatShader->Unbind();
        }

        m_cubemapsShader->Bind();
        m_skybox->GetCubemap()->Bind(0);
        m_cubemapsShader->SetUniform("u_skybox", 0);
        m_cubemapsShader->SetUniform("u_viewPosition", camera->GetWorldLocation());
        m_mirrorInstance->Draw(m_cubemapsShader, &transformMatrix.projection, &transformMatrix.view);
        m_cubemapsShader->Unbind();

#ifdef DEBUG_NORMALS
        m_debugNormalShader->Bind();
        m_bustInstance->Draw(m_debugNormalShader, &transformMatrix.projection, &transformMatrix.view);
        m_stanLeeInstance->Draw(m_debugNormalShader, &transformMatrix.projection, &transformMatrix.view);
        m_cubeInstance->Draw(m_debugNormalShader, &transformMatrix.projection, &transformMatrix.view);
        m_debugNormalShader->Unbind();
#endif
        m_scene->Unbind();

        EventTick();
    }

    void SandboxLayer::CheckInputs()
    {
        auto camera = Cast<EulerCamera>(m_scene->GetCamera());

        // AlphaEngine supports only US keyboard binding (aka QWERTY)
        if (Input::IsKeyPressed(ALPHA_KEY_W)) camera->MoveForward(1);
        if (Input::IsKeyPressed(ALPHA_KEY_S)) camera->MoveForward(-1);
        if (Input::IsKeyPressed(ALPHA_KEY_A)) camera->MoveRight(-1);
        if (Input::IsKeyPressed(ALPHA_KEY_D)) camera->MoveRight(1);

        if (Input::IsKeyPressed(ALPHA_KEY_C)) camera->SetZoom(DEFAULT_CAMERA_ZOOM);
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
    }

    void SandboxLayer::EventTick()
    {
        m_bustInstance->SetWorldRotation(m_bustInstance->GetWorldRotation() + Vector3(0, 0, 0.5f));
        m_mirrorInstance->SetWorldRotation(m_mirrorInstance->GetWorldRotation() + Vector3(0, 0.5f, 0));
        m_stanLeeInstance->SetWorldRotation(m_stanLeeInstance->GetWorldRotation() + Vector3(0, -2, 0));

        static float stanLeeTime = 4;
        static float stanLeeAngle = 0.0f;
        static float stanLeeDistance = 1.0f;
        Vector3 stanLeePosition = {stanLeeDistance * cos(stanLeeAngle), 0, stanLeeDistance * sin(stanLeeAngle)};
        m_stanLeeInstance->SetWorldLocation(m_bustInstance->GetWorldLocation() + stanLeePosition);

        stanLeeAngle += float(PI) / (60.0f * stanLeeTime);
    }

    TransformMatrix SandboxLayer::CalculateTransformMatrix()
    {
        auto camera = Cast<EulerCamera>(m_scene->GetCamera());

        float aspectRatio = m_scene->GetFramebuffer()->GetAspectRatio();
        Matrix4x4 projectionMatrix = camera->GetProjectionMatrix(aspectRatio);
        Matrix4x4 viewMatrix = MakeViewMatrix(camera->GetWorldLocation(), camera->GetWorldRotation());

        return {Matrix4x4(1), viewMatrix, projectionMatrix};
    }

    void SandboxLayer::DrawSkybox(const TransformMatrix& transformMatrix)
    {
        m_skyboxShader->Bind();
        Renderer::DisableDepthMask();

        Matrix4x4 skyboxView = Matrix4x4(Matrix3x3(transformMatrix.view)); // Remove any translation.
        m_skybox->Draw(m_skyboxShader, &transformMatrix.projection, &skyboxView);

        Renderer::EnableDepthMask();
        m_skyboxShader->Unbind();
    }

    void SandboxLayer::UpdateShadowMaps(const std::vector<Pointer<StaticMeshInstance>>& instances)
    {
        m_depthShader->Bind();
        for (const Pointer<Light>& light : m_scene->GetLights())
        {
            ALPHA_ASSERT(light->IsShadowingEnable(), "Cannot generate shadow map");

            light->GetDepthBuffer()->Bind();

            const Matrix4x4& lightSpace = light->CalculateViewProjectionMatrix();
            m_depthShader->SetUniform("u_lightSpace", lightSpace);

            for (const Pointer<StaticMeshInstance>& instance : instances)
            {
                instance->Draw(m_depthShader, nullptr, nullptr);
            }

            light->GetDepthBuffer()->Unbind();
        };
        m_depthShader->Unbind();
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
        const Pointer<Scene>& scene = GlobalStorage::GetScene("Scene_01");
        const Pointer<Framebuffer>& framebuffer = scene->GetFramebuffer();
        m_viewportWidget01.SetFramebuffer(framebuffer);
    }

    void GuiSandboxLayer::OnImGuiRender()
    {
        Pointer<Scene> scene = GlobalStorage::GetScene("Scene_01");

		m_sceneWidget.SetScene(scene);
		m_gizmoWidget.SetScene(scene);

		if (m_sceneWidget.IsSelectedEntityValid())
		{
            auto index = static_cast<uint32>(m_sceneWidget.GetSelectedEntityIndex());
			auto instance = Cast<StaticMeshInstance>(scene->GetComponentAt(index));

			if (instance != m_materialEditor.GetEntity()) m_materialEditor.SetEntity(instance);
            if (instance != m_gizmoWidget.GetMeshInstance()) m_gizmoWidget.SetMeshInstance(instance);
        }
		else
        {
		    m_materialEditor.Clear();
            m_gizmoWidget.Clear();
        }

        m_dockerWidget.Render();
        m_viewportWidget01.Render();
        m_statWidget.Render();
        m_sceneWidget.Render();
        m_materialEditor.Render();
        m_gizmoWidget.Render();
    }

    void GuiSandboxLayer::OnEvent(Event &e)
    {
        m_dockerWidget.OnEvent(e);
        m_viewportWidget01.OnEvent(e);
        m_statWidget.OnEvent(e);
        m_sceneWidget.OnEvent(e);
        m_materialEditor.OnEvent(e);
        m_gizmoWidget.OnEvent(e);
    }
}
