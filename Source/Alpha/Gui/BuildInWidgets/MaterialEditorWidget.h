#pragma once

#include <Alpha/Core/CoreMinimal.h>

#include <Alpha/ThirdParty/imgui/imgui.h>
#include <Alpha/Gui/ImGuiWidget.h>

#include <Alpha/Event/ApplicationEvent.h>

#include <Alpha/Engine/Material/Material.h>
#include <Alpha/Engine/Mesh/StaticMeshEntity.h>

namespace Alpha
{
    class MaterialEditorWidget : public ImGuiWidget
    {
    public:
        explicit MaterialEditorWidget(const std::string &name) : ImGuiWidget(name) { Init(); }

        void Init();
        void Render() override;

        void Clear();
        void InitFromStaticMeshEntity(const Pointer<StaticMeshEntity>& entity);

        inline bool IsOccupied() const { return m_bIsOccupied; }

    private:
        static void RenderMaterialNode(const Pointer<Material>& material);

    private:
        bool m_bIsOccupied = false;

        std::vector<Pointer<Material>> m_materials = {};
    };
}