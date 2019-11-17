#pragma once

#include <Alpha/Core/CoreMinimal.h>

#include <Alpha/ThirdParty/imgui/imgui.h>
#include <Alpha/Gui/ImGuiWidget.h>

#include <Alpha/Event/ApplicationEvent.h>

#include <Alpha/Engine/Scene/Scene.h>
#include <Alpha/Engine/Camera/Camera.h>
#include <Alpha/Engine/Mesh/StaticMeshInstance.h>

namespace Alpha
{
    class ImGuizmoWidget : public ImGuiWidget
    {
    public:
        explicit ImGuizmoWidget(const std::string &name) : ImGuiWidget(name) { Init(); }

        void Init();
        void Render() override;

        inline const Pointer<StaticMeshInstance>& GetMeshInstance() const { return m_instance; }
        inline void SetMeshInstance(const Pointer<StaticMeshInstance>& instance) { m_instance = instance; }

        inline const Pointer<Scene>& GetScene() const { return m_scene; }
        inline void SetScene(const Pointer<Scene>& scene) { m_scene = scene; }

        void Clear();

    private:
        Pointer<Scene> m_scene;
        Pointer<StaticMeshInstance> m_instance;
    };
}