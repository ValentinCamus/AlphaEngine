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
        EulerCamera m_camera;
        Pointer<Shader> m_pbrShader;

        Pointer<Texture2D> m_brickTexture;

        Pointer<StaticMeshModel> m_cube;
        Pointer<StaticMeshModel> m_stanfordDragon;

        Pointer<StaticMeshEntity> m_stanfordDragonInstance;

        Pointer<Light> m_directionalLight;

        BSpline m_spline;
        Pointer<StaticMeshModel> m_splineLineModel;
        Pointer<StaticMeshEntity> m_splineLineEntity;
        Pointer<StaticMeshModel> m_splinePointsModel;
        Pointer<StaticMeshEntity> m_splinePointsEntity;

        TensorProduct m_tensorProduct;
        Pointer<StaticMeshModel> m_tensorMeshModel;
        Pointer<StaticMeshEntity> m_tensorMeshEntity;
        Pointer<StaticMeshModel> m_tensorPointsModel;
        Pointer<StaticMeshEntity> m_tensorPointsEntity;
    };

    class GuiSandboxLayer : public ImGuiLayer
    {
    public:
        explicit GuiSandboxLayer();

        void OnImGuiRender() override;

        void OnEvent(Event& e) override;

    private:
        DockerWidget m_docker = DockerWidget("Docker Widget");
        ViewportWidget m_viewport01 = ViewportWidget("Viewport-01");
        StatsWidget m_stats = StatsWidget("Stats Widget");
        SceneWidget m_scene = SceneWidget("Scene Widget");
        MaterialEditorWidget m_materialEditor = MaterialEditorWidget("Material Editor");

    };
}
