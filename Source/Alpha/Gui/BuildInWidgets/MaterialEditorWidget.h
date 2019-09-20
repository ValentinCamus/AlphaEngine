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
        struct MaterialInfo
        {
            int32 type = 0;

            ImVec4 kd = ImVec4(1, 1, 1, 1);
            ImVec4 ks = ImVec4(1, 1, 1, 1);
            float roughness = 0.2f;
            float metallic = 0.1f;
            float transparency = 1.0f;
        };

    public:
        explicit MaterialEditorWidget(const std::string &name) : ImGuiWidget(name) {}

        void Render() override;

        void Clear();

        inline const Pointer<StaticMeshEntity>& GetEntity() const { return m_entity; }
        void SetEntity(const Pointer<StaticMeshEntity>& entity);

        inline bool IsOccupied() const { return m_bIsOccupied; }

    private:
        void RenderMaterialNode(const Pointer<Material>& material);

    private:
        bool m_bIsOccupied = false;
        bool m_bNeedUpdate = false;

        Pointer<StaticMeshEntity> m_entity = nullptr;
        std::vector<Pointer<Material>> m_materials;
        MaterialInfo m_materialInfo;
    };
}