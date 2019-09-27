#pragma once

#include <Alpha/ThirdParty/imgui/imgui.h>

#include <Alpha/Layer/Layer.h>
#include <Alpha/Gui/ImGuiLayer.h>

#include <Alpha/Engine/Renderer/Renderer.h>
#include <Alpha/Engine/Renderer/Framebuffer.h>

#include <Alpha/Gui/BuildInWidgets/StatsWidget.h>
#include <Alpha/Gui/BuildInWidgets/DockerWidget.h>
#include <Alpha/Gui/BuildInWidgets/ViewportWidget.h>
#include <Alpha/Gui/BuildInWidgets/SceneWidget.h>
#include <Alpha/Gui/BuildInWidgets/MaterialEditorWidget.h>

#include <Alpha/Input/Input.h>

#include <Alpha/Engine/Renderer/Shader.h>

#include <Alpha/Engine/Camera/EulerCamera.h>

#include <Alpha/Engine/Mesh/StaticMeshModel.h>
#include <Alpha/Engine/Mesh/StaticMeshEntity.h>

#include <Alpha/Engine/Renderer/Texture.h>
#include <Alpha/Engine/Material/Material.h>

#include <Alpha/Engine/Renderer/OpenGL/OpenGLRenderer.h>


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
        EulerCamera m_camera;
        Pointer<Shader> m_pbrShader;

        Pointer<Texture2D> m_brickTexture;

        Pointer<StaticMeshModel> m_cube;
        Pointer<StaticMeshModel> m_stanfordDragon;

        Pointer<StaticMeshEntity> m_stanfordDragonInstance;

        Pointer<Light> m_directionalLight;

        BSpline m_spline;
        std::vector<Pointer<StaticMeshEntity>> m_splineNodes;
        std::vector<Pointer<StaticMeshEntity>> m_splineControlPoints;

        TensorProduct m_tensorProduct;
        std::vector<Pointer<StaticMeshEntity>> m_tensorProductNodes;
        std::vector<Pointer<StaticMeshEntity>> m_tensorProductControlPoints;
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
