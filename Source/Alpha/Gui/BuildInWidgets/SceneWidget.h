#pragma once

#include <Alpha/Core/CoreMinimal.h>

#include <Alpha/ThirdParty/imgui/imgui.h>
#include <Alpha/Gui/ImGuiWidget.h>

#include <Alpha/Event/ApplicationEvent.h>

#include <Alpha/Engine/Scene/Scene.h>

namespace Alpha
{
    class SceneWidget : public ImGuiWidget
    {
    public:
        explicit SceneWidget(const std::string &name) : ImGuiWidget(name) { Init(); }

        void Init();
        void Render() override;

        inline int32 GetSelectedEntityIndex() const { return m_selectedEntityIndex - 1; }
		inline bool IsSelectedEntityValid() const { return m_selectedEntityIndex > 0; }


		inline const Pointer<Scene>& GetScene() const { return m_scene; }
		inline void SetScene(const Pointer<Scene>& scene) { m_scene = scene; }

    private:
		Pointer<Scene> m_scene = nullptr;

        std::vector<const char*> m_components;
        int32 m_selectedEntityIndex = 0;
    };
}