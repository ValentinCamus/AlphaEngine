#pragma once

#include <Alpha/Core/CoreMinimal.h>

#include <Alpha/ThirdParty/imgui/imgui.h>
#include <Alpha/Gui/ImGuiWidget.h>

#include <Alpha/Event/ApplicationEvent.h>

namespace Alpha
{
    class SceneWidget : public ImGuiWidget
    {
    public:
        explicit SceneWidget(const std::string &name) : ImGuiWidget(name) { Init(); }

        void Init();
        void Render() override;

        void PushEntity(const char* name);

        inline int32 GetSelectedEntityIndex() const { return m_selectedEntityIndex; }

    private:
        std::vector<const char*> m_entities = {};
        int32 m_selectedEntityIndex = 0;
    };
}