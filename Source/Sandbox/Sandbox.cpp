#include "Sandbox.h"

#include <Alpha/Engine/Light/DirectionalLight.h>

namespace Alpha
{
    void SandboxLayer::Init()
    {
        Pointer<EulerCamera> camera = NewPointer<EulerCamera>();

        Pointer<DirectionalLight> sunLight = NewPointer<DirectionalLight>();
        sunLight->SetColor(Color4(1.0f, 1.0f, 0.7f, 1.0f));

        m_flatShader = Shader::Create("Flat", {
                {Shader::GLSL_VERTEX_SHADER, ALPHA_SHADERS_DIR + "Flat.vs.glsl"},
                {Shader::GLSL_FRAGMENT_SHADER, ALPHA_SHADERS_DIR + "Flat.fs.glsl"}
        });
        m_forwardShader = Shader::Create("Forward", {
                {Shader::GLSL_VERTEX_SHADER, ALPHA_SHADERS_DIR + "Forward.vs.glsl"},
                {Shader::GLSL_FRAGMENT_SHADER, ALPHA_SHADERS_DIR + "Forward.fs.glsl"}
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
		m_scene->PushLight(sunLight);
		m_scene->SetFramebuffer(Framebuffer::Create(500, 500));
		m_scene->SetCamera(camera);

        Pointer<Texture2D> brickTexture = Texture2D::Create(ALPHA_ASSETS_DIR + "Brick.jpg");

        Pointer<Material> defaultMaterial = NewPointer<Material>("DefaultMaterial");
        Pointer<Material> redMaterial = NewPointer<Material>("RedMaterial");
        Pointer<Material> brickMaterial = NewPointer<Material>("BrickMaterial");
        Pointer<Material> lightMaterial = NewPointer<Material>("LightMaterial");

        redMaterial->SetKd(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
        brickMaterial->AddTexture(ETextureType::TX_Diffuse, brickTexture);

        Pointer<StaticMeshModel> tileModel = StaticMeshModel::Create(ALPHA_ASSETS_DIR + "Tile.fbx");
        Pointer<StaticMeshModel> cubeModel = StaticMeshModel::Create(ALPHA_ASSETS_DIR + "Cube.fbx");
		Pointer<StaticMeshModel> dragonModel = StaticMeshModel::Create(ALPHA_ASSETS_DIR + "StanfordDragon.fbx");

        m_screenInstance = NewPointer<StaticMeshInstance>("Tile", tileModel);
        m_screenInstance->SetMaterial(0, defaultMaterial);
        m_screenInstance->SetWorldLocation({-2, 0, -3});
        m_screenInstance->SetWorldRotation({0, 45, 0});

        m_lightInstance = NewPointer<StaticMeshInstance>("Cube", cubeModel);
        m_lightInstance->SetMaterial(0, lightMaterial);
        m_lightInstance->SetWorldScale({0.1f, 0.1f, 0.1f});

        m_dragonInstance = NewPointer<StaticMeshInstance>("Dragon", dragonModel);
        m_dragonInstance->SetMaterial(0, defaultMaterial);
        m_dragonInstance->SetMaterial(1, brickMaterial);
        m_dragonInstance->SetWorldLocation({0, -1, -2});
        m_dragonInstance->SetWorldScale({0.05, 0.05, 0.05});

        m_scene->PushComponent(m_screenInstance);
		m_scene->PushComponent(m_dragonInstance);

        GlobalStorage::AddScene("Scene_01", m_scene);

        GlobalStorage::AddShader("Flat", m_flatShader);
        GlobalStorage::AddShader("Depth", m_depthShader);
        GlobalStorage::AddShader("Forward", m_forwardShader);
        GlobalStorage::AddShader("DebugDepth", m_debugDepthShader);
        GlobalStorage::AddShader("DebugNormal", m_debugNormalShader);

        GlobalStorage::AddTexture2D("Brick", brickTexture);

        GlobalStorage::AddMaterial("Default", defaultMaterial);
        GlobalStorage::AddMaterial("Red", redMaterial);
        GlobalStorage::AddMaterial("Brick", brickMaterial);
        GlobalStorage::AddMaterial("Light", lightMaterial);

        GlobalStorage::AddStaticMeshModel("Cube", cubeModel);
        GlobalStorage::AddStaticMeshModel("Tile", tileModel);
        GlobalStorage::AddStaticMeshModel("StanfordDragon", dragonModel);

        LogTips();

        InitShadowMaps(m_scene->GetLights());
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
        ALPHA_ASSERT(m_scene->GetFramebuffer(), "Invalid Framebuffer");

        for (const Pointer<Light>& light : m_scene->GetLights()) RenderShadowMap(light);

        TransformMatrix transformMatrix = MakeTransformMatrix(m_scene);

        m_scene->Bind();

        Renderer::SetClearColor({0.2f, 0.3f, 0.3f, 1.0f});
        Renderer::Clear();

        for (const Pointer<Light>& light : m_scene->GetLights())
        {
            m_forwardShader->Bind();

            m_forwardShader->SetUniform("u_light", light);
            m_forwardShader->SetUniform("u_viewPosition", camera->GetWorldLocation());

            m_dragonInstance->Draw(m_forwardShader, transformMatrix);

            m_forwardShader->Unbind();

            m_debugDepthShader->Bind();

            Pointer<Texture2D> shadowMap = light->GetDepthBuffer()->GetTexture();

            shadowMap->Bind(0);
            m_debugDepthShader->SetUniform("u_depthMap", shadowMap->GetSlot());

            Renderer::GetDrawOptions()->bUseMaterial = false;

            m_screenInstance->Draw(m_debugDepthShader, transformMatrix);

            Renderer::GetDrawOptions()->Reset();

            m_debugDepthShader->Unbind();

            // Draw3DNormals(m_scene);
            DrawSceneLights(m_scene);
        }

        m_dragonInstance->SetWorldRotation(m_dragonInstance->GetWorldRotation() + Vector3(0, 0.5f, 0));

        m_scene->Unbind();
    }

    Matrix4x4 SandboxLayer::RenderShadowMap(const Pointer<Light> &light)
    {
        using EViewType = Camera::EViewType;

        constexpr float NEAR_PLANE = 1.0f;
        constexpr float FAR_PLANE = 7.5f;

        TransformMatrix nullTransform = {Matrix4x4(1), Matrix4x4(1), Matrix4x4(1)};

        ALPHA_ASSERT(light->IsDepthBufferValid(), "Invalid Shadow Map");

        m_depthShader->Bind();
        light->GetDepthBuffer()->Bind();

        bool bIsDirLight = InstanceOf<DirectionalLight>(light);

        Camera lightCamera = Camera(light->GetWorldLocation(), light->GetWorldRotation());
        lightCamera.SetViewType(bIsDirLight ? EViewType::VT_Orthographic : EViewType::VT_Perspective);

        Matrix4x4 lightProjection = Matrix4x4(1.0f);
        float aspectRatio = light->GetDepthBuffer()->GetAspectRatio();

        switch (lightCamera.GetViewType())
        {
            case Camera::EViewType::VT_Orthographic:
            {
                lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, NEAR_PLANE, FAR_PLANE);
                break;
            }
            case Camera::EViewType::VT_Perspective:
            {
                float fovy = glm::radians(lightCamera.GetZoom());
                lightProjection = glm::perspective(fovy, aspectRatio, NEAR_PLANE, FAR_PLANE);
                break;
            }
        }

        Matrix4x4 lightView = MakeViewMatrix(lightCamera.GetWorldLocation(), lightCamera.GetWorldRotation());
        Matrix4x4 lightSpace = lightProjection * lightView;

        m_depthShader->SetUniform("u_lightSpace", lightSpace);

        Renderer::GetDrawOptions()->bUseMaterial = false;
        Renderer::GetDrawOptions()->bUseViewMatrix = false;
        Renderer::GetDrawOptions()->bUseProjectionMatrix = false;

        m_dragonInstance->Draw(m_depthShader, nullTransform);

        Renderer::GetDrawOptions()->Reset();

        light->SetSpace(lightSpace);

        light->GetDepthBuffer()->Unbind();
        m_depthShader->Unbind();

        return lightSpace;
    }

    void SandboxLayer::InitShadowMaps(const std::vector<Pointer<Light>> &lights)
    {
        for (const Pointer<Light>& light : lights)
        {
            Pointer<DepthBuffer> depthBuffer = DepthBuffer::Create(1024, 1024);

            light->SetDepthBuffer(depthBuffer);

            ALPHA_ASSERT(light->IsDepthBufferValid(), "Failed to initialize Shadow Map");
        }
    }

    void SandboxLayer::Draw3DNormals(const Pointer<Scene> &scene)
    {
        TransformMatrix transformMatrix = MakeTransformMatrix(scene);

        m_debugNormalShader->Bind();

        Renderer::GetDrawOptions()->bUseMaterial = false;

        m_screenInstance->Draw(m_debugNormalShader, transformMatrix);
        m_lightInstance->Draw(m_debugNormalShader, transformMatrix);
        m_dragonInstance->Draw(m_debugNormalShader, transformMatrix);

        Renderer::GetDrawOptions()->Reset();

        m_debugNormalShader->Unbind();
    }

    void SandboxLayer::DrawSceneLights(const Pointer<Scene> &scene)
    {
        scene->Bind();

        TransformMatrix transformMatrix = MakeTransformMatrix(scene);

        Renderer::GetDrawOptions()->bUseMaterial = false;

        for (const Pointer<Light>& light : scene->GetLights())
        {
            m_lightInstance->SetWorldLocation(light->GetWorldLocation());
            m_lightInstance->SetWorldRotation(light->GetWorldRotation());

            m_flatShader->Bind();

            m_flatShader->SetUniform("u_color", light->GetColor());

            m_lightInstance->Draw(m_flatShader, transformMatrix);

            m_flatShader->Unbind();

        }

        Renderer::GetDrawOptions()->Reset();

        scene->Unbind();
    }

    TransformMatrix SandboxLayer::MakeTransformMatrix(const Pointer<Scene> &scene)
    {
        const Pointer<Camera>& camera = scene->GetCamera();

        Matrix4x4 projectionMatrix = Matrix4x4(1.0f);
        float aspectRatio = scene->GetFramebuffer()->GetAspectRatio();

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

        return transformMatrix;
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

		if (m_sceneWidget.IsSelectedEntityValid())
		{
            auto index = static_cast<uint32>(m_sceneWidget.GetSelectedEntityIndex());
			auto entity = Cast<StaticMeshInstance>(scene->GetComponentAt(index));
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
