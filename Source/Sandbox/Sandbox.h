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

		void RenderShadowMaps();

    private:
		Pointer<Scene> m_scene;
        Pointer<Shader> m_shader;

        Pointer<Scene> m_depthScene;
        Pointer<Shader> m_depthShader;
        Pointer<Shader> m_debugDepthShader;

        Pointer<StaticMeshEntity> m_cubeInstance;
        Pointer<StaticMeshEntity> m_tileInstance;
        Pointer<StaticMeshEntity> m_stanfordDragonInstance;
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
