#include "MaterialEditorWidget.h"

namespace Alpha
{
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

        if (m_entity)
        {
            static int32 materialIndex = 0;

            if (m_bNeedUpdate) materialIndex = 0;

            int32 lastMaterialIndex = materialIndex;
            int32 nMaterials = m_entity->GetNbMaterial() == 0 ? 0 : m_entity->GetNbMaterial() - 1;
            ImGui::SliderInt("Material Index", &materialIndex, 0, nMaterials);
            ImGui::NewLine();

            if (materialIndex != lastMaterialIndex || m_bNeedUpdate)
            {
                m_bNeedUpdate = false;
                Pointer<Material> material = m_entity->GetMaterial(static_cast<uint32>(materialIndex));

                m_materials.push_back(material);
                m_materialInfo = {
                        static_cast<int32>(material->GetType()),
                        {material->GetKd().x, material->GetKd().y, material->GetKd().z, material->GetKd().a},
                        {material->GetKs().x, material->GetKs().y, material->GetKs().z, material->GetKs().a},
                        material->GetRoughness(),
                        material->GetMetallic(),
                        material->GetTransparency()
                };
            }

            Pointer<Material> material = m_materials.at(static_cast<unsigned long>(materialIndex));
            const std::string& name = material->GetName();

            ImGui::Separator();
            ImGui::Text("%s (Index = %d)", ToCharArray(name), materialIndex);
            ImGui::Separator();
            ImGui::NewLine();
            RenderMaterialNode();

            material->SetType(static_cast<EMaterialType>(m_materialInfo.type));

            material->SetKd({
                    m_materialInfo.kd.x,
                    m_materialInfo.kd.y,
                    m_materialInfo.kd.z,
                    m_materialInfo.kd.w
            });

            material->SetKs({
                    m_materialInfo.ks.x,
                    m_materialInfo.ks.y,
                    m_materialInfo.ks.z,
                    m_materialInfo.ks.w
            });

            material->SetRoughness(m_materialInfo.roughness);
            material->SetMetallic(m_materialInfo.metallic);
            material->SetTransparency(m_materialInfo.transparency);
        }
        else ImGui::Text("<No entity selected>");

        ImGui::End();
    }

    void MaterialEditorWidget::Clear()
    {
        m_bIsVisible = false;
        m_bIsOccupied = false;
        m_materials.clear();
        m_entity = nullptr;
        m_materials.clear();
    }

    void MaterialEditorWidget::SetEntity(const Pointer<StaticMeshInstance>& entity)
    {
        m_bIsVisible = true;
        m_bIsOccupied = true;
        m_entity = entity;
        m_bNeedUpdate = true;

        for (uint32 i = 0; i < entity->GetNbMaterial(); ++i)
        {
            const Pointer<Material>& material = entity->GetMaterial(i);
            m_materials.push_back(material);
        }
    }

    void MaterialEditorWidget::RenderMaterialNode()
    {
        const char* items[] = {"Opaque", "Transparent"};
        ImGui::Combo("Type", &m_materialInfo.type, items, IM_ARRAYSIZE(items));
        ImGui::NewLine();

        if (ImGui::TreeNode("Albedo/Diffuse"))
        {
            ImGui::ColorPicker4("Diffuse", (float*)&(m_materialInfo.kd), 0);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Specular"))
        {
            ImGui::ColorPicker4("Diffuse", (float*)&(m_materialInfo.ks), 0);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Normal"))
        {
            ImGui::Text("<Normal>");
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Roughness"))
        {

            ImGui::InputFloat("Roughness", &m_materialInfo.roughness, 0.01f, 1.0f, "%.3f");
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Metallic"))
        {
            ImGui::InputFloat("Metallic", &m_materialInfo.metallic, 0.01f, 1.0f, "%.3f");
            ImGui::TreePop();
        }

        if (m_materialInfo.type == 1)
        {
            if (ImGui::TreeNode("Transparency"))
            {
                ImGui::InputFloat("Transparency", &m_materialInfo.transparency, 0.01f, 1.0f, "%.3f");
                ImGui::TreePop();
            }
        } else m_materialInfo.transparency = 1.0f;

        ImGui::NewLine();
    }
}