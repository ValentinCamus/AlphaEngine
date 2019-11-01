#pragma once

#include <Alpha/Layer/Layer.h>
#include <Alpha/Gui/ImGuiLayer.h>

#include <Alpha/Core/CoreMinimal.h>
#include <Alpha/Engine/EngineMinimal.h>

#include <Alpha/Gui/BuildInWidgets/StatsWidget.h>
#include <Alpha/Gui/BuildInWidgets/DockerWidget.h>
#include <Alpha/Gui/BuildInWidgets/ViewportWidget.h>
#include <Alpha/Gui/BuildInWidgets/SceneWidget.h>
#include <Alpha/Gui/BuildInWidgets/MaterialEditorWidget.h>

#include <Alpha/Input/Input.h>

namespace Alpha
{
    class SandboxLayer : public Layer
    {
    public:
        explicit inline SandboxLayer() : Layer("Sandbox Layer") { Init(); }

        void Init();

        void OnUpdate() override;

	private:
		void LogTips();


		TransformMatrix MakeTransformMatrix(const Pointer<Scene> &scene);

		void InitShadowMaps(const std::vector<Pointer<Light>> &lights);
        Matrix4x4 RenderShadowMap(const Pointer<Light> &light);

        void DrawSceneLights(const Pointer<Scene> &scene);
        void Draw3DNormals(const Pointer<Scene> &scene);

    private:
		Pointer<Scene> m_scene;

		Pointer<Shader> m_flatShader;
        Pointer<Shader> m_depthShader;
        Pointer<Shader> m_forwardShader;
        Pointer<Shader> m_debugDepthShader;
        Pointer<Shader> m_debugNormalShader;

		Pointer<StaticMeshInstance> m_lightInstance;
        Pointer<StaticMeshInstance> m_screenInstance;
        Pointer<StaticMeshInstance> m_dragonInstance;
    };

    class GuiSandboxLayer : public ImGuiLayer
    {
    public:
        explicit GuiSandboxLayer();

        void OnImGuiRender() override;

        void OnEvent(Event& e) override;

    private:
        DockerWidget m_dockerWidget = DockerWidget("Docker Widget");
        ViewportWidget m_viewportWidget01 = ViewportWidget("Viewport-01");
        StatsWidget m_statWidget = StatsWidget("Stats Widget");
        SceneWidget m_sceneWidget = SceneWidget("Scene Widget");
        MaterialEditorWidget m_materialEditor = MaterialEditorWidget("Material Editor");

    };
}
