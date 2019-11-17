#pragma once

#include <Alpha/Layer/Layer.h>
#include <Alpha/Gui/ImGuiLayer.h>

#include <Alpha/Core/CoreMinimal.h>
#include <Alpha/Engine/EngineMinimal.h>

#include <Alpha/Gui/BuildInWidgets/SceneWidget.h>
#include <Alpha/Gui/BuildInWidgets/StatsWidget.h>
#include <Alpha/Gui/BuildInWidgets/DockerWidget.h>
#include <Alpha/Gui/BuildInWidgets/ViewportWidget.h>
#include <Alpha/Gui/BuildInWidgets/ImGuizmoWidget.h>
#include <Alpha/Gui/BuildInWidgets/MaterialEditorWidget.h>

#include <Alpha/Input/Input.h>

#include <Alpha/Engine/BSpline/Nurbs.h>
#include <Alpha/Engine/BSpline/BSplineController.h>

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

    private:
		Pointer<Scene> m_scene = nullptr;

		Pointer<Skybox> m_skybox = nullptr;

		Pointer<Shader> m_flatShader = nullptr;
        Pointer<Shader> m_depthShader = nullptr;
        Pointer<Shader> m_forwardShader = nullptr;
		Pointer<Shader> m_skyboxShader = nullptr;
		Pointer<Shader> m_cubemapsShader = nullptr;
        Pointer<Shader> m_debugDepthShader = nullptr;
        Pointer<Shader> m_debugNormalShader = nullptr;

		Pointer<StaticMeshInstance> m_cubeInstance = nullptr;
		Pointer<StaticMeshInstance> m_lightInstance = nullptr;
        Pointer<StaticMeshInstance> m_planeInstance = nullptr;
        Pointer<StaticMeshInstance> m_dragonInstance = nullptr;
		Pointer<StaticMeshInstance> m_mirrorInstance = nullptr;

		Pointer<BSplineController> m_bSplineController = nullptr;
    };

    class GuiSandboxLayer : public ImGuiLayer
    {
    public:
        explicit GuiSandboxLayer();

        void OnImGuiRender() override;

        void OnEvent(Event& e) override;

    private:
		ImGuizmoWidget m_gizmoWidget = ImGuizmoWidget("ImGuizmo Widget");
        DockerWidget m_dockerWidget = DockerWidget("Docker Widget");
        ViewportWidget m_viewportWidget01 = ViewportWidget("Viewport-01");
        StatsWidget m_statWidget = StatsWidget("Stats Widget");
        SceneWidget m_sceneWidget = SceneWidget("Scene Widget");
        MaterialEditorWidget m_materialEditor = MaterialEditorWidget("Material Editor");
    };
}
