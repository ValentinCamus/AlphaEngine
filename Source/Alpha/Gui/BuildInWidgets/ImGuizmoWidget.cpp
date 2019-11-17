#include "ImGuizmoWidget.h"

#include <Alpha/ThirdParty/imguizmo/ImGuizmo.h>

namespace Alpha
{
    void ImGuizmoWidget::Init()
    {

    }

    void ImGuizmoWidget::Render()
    {
        ImGuizmo::BeginFrame();

        ImGui::SetWindowPos(ImVec2(64, 64), ImGuiCond_FirstUseEver);
        ImGui::SetWindowSize(ImVec2(512, 512), ImGuiCond_FirstUseEver);

        if (!ImGui::Begin(ToCharArray(GetName()), &m_bIsVisible))
        {
            // Early out if the window is collapsed, as an optimization.
            ImGui::End();
            return;
        }

        ImGui::Text("Scene Entities: ");
        if (m_scene->GetCamera())
        {
            Pointer<Camera> camera = m_scene->GetCamera();

            Matrix4x4 view = MakeViewMatrix(camera->GetWorldLocation(), camera->GetWorldRotation());
            float * imGuizmoView = glm::value_ptr(view);

            Matrix4x4 proj = camera->GetProjectionMatrix(m_scene->GetFramebuffer()->GetAspectRatio());
            float * imGuizmoProj = glm::value_ptr(proj);

            if (m_instance)
            {
                float tr[3], rt[3], sc[3];

                const Vector3& location = m_instance->GetWorldLocation();
                const Vector3& rotation = m_instance->GetWorldRotation();
                const Vector3& scale = m_instance->GetWorldScale();

                Matrix4x4 model = MakeModelMatrix(location, rotation, scale);
                float * imGuizmoModel = glm::value_ptr(model);

                static ImGuizmo::OPERATION currentGizmoOperation(ImGuizmo::TRANSLATE);
                static ImGuizmo::MODE currentGizmoMode(ImGuizmo::WORLD);

                if (ImGui::IsKeyPressed(90)) currentGizmoOperation = ImGuizmo::TRANSLATE;
                if (ImGui::IsKeyPressed(69)) currentGizmoOperation = ImGuizmo::ROTATE;
                if (ImGui::IsKeyPressed(82)) currentGizmoOperation = ImGuizmo::SCALE;

                if (ImGui::RadioButton("Translate", currentGizmoOperation == ImGuizmo::TRANSLATE))
                {
                    currentGizmoOperation = ImGuizmo::TRANSLATE;
                }
                ImGui::SameLine();
                if (ImGui::RadioButton("Rotate", currentGizmoOperation == ImGuizmo::ROTATE))
                {
                    currentGizmoOperation = ImGuizmo::ROTATE;
                }
                ImGui::SameLine();
                if (ImGui::RadioButton("Scale", currentGizmoOperation == ImGuizmo::SCALE))
                {
                    currentGizmoOperation = ImGuizmo::SCALE;
                }

                ImGuizmo::DecomposeMatrixToComponents(imGuizmoModel, tr, rt, sc);
                rt[0] = m_instance->GetWorldRotation()[0];
                rt[1] = m_instance->GetWorldRotation()[1];
                rt[2] = m_instance->GetWorldRotation()[2];
                ImGui::InputFloat3("Tr", tr, 3);
                ImGui::InputFloat3("Rt", rt, 3);
                ImGui::InputFloat3("Sc", sc, 3);
                m_instance->SetWorldRotation({rt[0], rt[1], rt[2]});
                ImGuizmo::RecomposeMatrixFromComponents(tr, rt, sc, imGuizmoModel);

                if (currentGizmoOperation != ImGuizmo::SCALE)
                {
                    if (ImGui::RadioButton("Local", currentGizmoMode == ImGuizmo::LOCAL))
                    {
                        currentGizmoMode = ImGuizmo::LOCAL;
                    }
                    ImGui::SameLine();
                    if (ImGui::RadioButton("World", currentGizmoMode == ImGuizmo::WORLD))
                    {
                        currentGizmoMode = ImGuizmo::WORLD;
                    }
                }

                ImGuizmo::UpdateEulerAngles(m_instance->GetWorldRotation().x,
                                            m_instance->GetWorldRotation().y,
                                            m_instance->GetWorldRotation().z);

                ImGuiIO& io = ImGui::GetIO();
                ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
                ImGuizmo::Manipulate(imGuizmoView, imGuizmoProj, currentGizmoOperation, currentGizmoMode, imGuizmoModel);

                ImGuizmo::DecomposeMatrixToComponents(imGuizmoModel, tr, rt, sc);
                std::array<float, 3> eulerAngles = ImGuizmo::GetEulerAngles();
                m_instance->SetWorldLocation({tr[0], tr[1], tr[2]});
                m_instance->SetWorldRotation({eulerAngles[0], eulerAngles[1], eulerAngles[2]});
                m_instance->SetWorldScale({sc[0], sc[1], sc[2]});
            }
            else ImGui::Text("<No instance selected>");
        }

        ImGui::End();
    }

    void ImGuizmoWidget::Clear()
    {
        m_instance = nullptr;
        ImGuizmo::ClearEulerAngles();
    }
}