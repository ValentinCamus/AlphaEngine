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

        void InitBSplineExample();
        void InitTensorProductExample();

	private:
		void LogTips();

    private:
		Pointer<Scene> m_scene;
        Pointer<Shader> m_shader;

        Pointer<StaticMeshEntity> m_stanfordDragonInstance;

        BSpline m_spline;
        Pointer<StaticMeshEntity> m_splineLineEntity;
        Pointer<StaticMeshEntity> m_splinePointsEntity;

        TensorProduct m_tensorProduct;
        Pointer<StaticMeshEntity> m_tensorMeshEntity;
        Pointer<StaticMeshEntity> m_tensorPointsEntity;
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
