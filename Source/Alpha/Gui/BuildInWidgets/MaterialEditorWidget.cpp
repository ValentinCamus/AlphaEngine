#include "MaterialEditorWidget.h"

namespace Alpha
{

    void MaterialEditorWidget::Init()
    {

    }

    void MaterialEditorWidget::Render()
    {
        ImGui::SetWindowPos(ImVec2(64, 64), ImGuiCond_FirstUseEver);
        ImGui::SetWindowSize(ImVec2(512, 512), ImGuiCond_FirstUseEver);

        if (!ImGui::Begin(ToCharArray(GetName()), &m_bIsVisible))
        {
            // Early out if the window is collapsed, as an optimization.
            ImGui::End();
            return;
        }

        for (const Pointer<Material>& material : m_materials)
        {
            ImGui::Separator();
            ImGui::Text("%s", ToCharArray(material->GetName()));
            ImGui::Separator();
            ImGui::NewLine();
            RenderMaterialNode(material);
        }

        ImGui::End();
    }

    void MaterialEditorWidget::Clear()
    {
        m_bIsVisible = false;
        m_bIsOccupied = false;
        m_materials.clear();
    }

    void MaterialEditorWidget::InitFromStaticMeshEntity(const Pointer<StaticMeshEntity>& entity)
    {
        m_bIsVisible = true;
        m_bIsOccupied = true;

        uint32 nMaterials = entity->GetNbMaterial();

        for (uint32 i = 0; i < nMaterials; ++i) m_materials.push_back(entity->GetMaterial(i));
    }

    void MaterialEditorWidget::RenderMaterialNode(const Pointer<Material> &material)
    {
        static int32 currentItem = 0;
        const char* items[] = {"Opaque", "Transparent"};
        ImGui::Combo("Type", &currentItem, items, IM_ARRAYSIZE(items));
        ImGui::NewLine();

        if (ImGui::TreeNode("Albedo/Diffuse"))
        {
            ImGui::Text("<Albedo/Diffuse>");
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Specular"))
        {
            ImGui::Text("<Specular>");
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Normal"))
        {
            ImGui::Text("<Normal>");
            ImGui::TreePop();
        }


        if (ImGui::TreeNode("Roughness"))
        {
            ImGui::Text("<Roughness>");
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Metallic"))
        {
            ImGui::Text("<Metallic>");
            ImGui::TreePop();
        }

        if (currentItem == 1)
        {
            if (ImGui::TreeNode("Transparency"))
            {
                ImGui::Text("<Transparency>");
                ImGui::TreePop();
            }
        }

        ImGui::NewLine();
    }
}