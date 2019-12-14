#include "BSplineController.h"

#include <Alpha/Engine/GlobalStorage/GlobalStorage.h>

namespace Alpha
{
    Pointer<Material> BSplineController::s_nodeMaterial = nullptr;
    Pointer<StaticMeshModel> BSplineController::s_nodeModel = nullptr;

    void BSplineController::Init()
    {
        if (!s_nodeModel)
        {
            Pointer<Material> material = NewPointer<Material>("BSplineNodeMaterial");
            material->SetKd(Vector4(1.0f, 1.0f, 0.0f, 1.0f));
            GlobalStorage::AddMaterial("BSplineNode", material);

            s_nodeMaterial = material;
            s_nodeModel = GlobalStorage::GetStaticMeshModel("Sphere");
        }
    }

    void BSplineController::Draw(const Pointer<Shader> &shader,
                                 const Matrix4x4 *projection,
                                 const Matrix4x4 *view) const
    {
        bool bIsValid = m_bSpline->GetNbPoints() == m_nodeInstances.size();
        ALPHA_ASSERT(bIsValid, "BSplineController::Draw: Missing UpdateNodeInstances call");

        for (uint32 i = 0; i < m_bSpline->GetNbPoints(); ++i)
        {
            Pointer<StaticMeshInstance> instance = m_nodeInstances[i];
            instance->Draw(shader, projection, view);
        }

        if (IsBSplineCurveValid()) m_curve->Draw(shader, projection, view);
    }

    void BSplineController::Draw(const Pointer<Shader> &shader,
                                 const Matrix4x4 *projection,
                                 const Matrix4x4 *view,
                                 const Matrix4x4 *model) const
    {
        bool bIsValid = m_bSpline->GetNbPoints() == m_nodeInstances.size();
        ALPHA_ASSERT(bIsValid, "BSplineController::Draw: Missing UpdateNodeInstances call");

        for (uint32 i = 0; i < m_bSpline->GetNbPoints(); ++i)
        {
            Pointer<StaticMeshInstance> instance = m_nodeInstances[i];
            instance->Draw(shader, projection, view, model);
        }

        if (IsBSplineCurveValid()) m_curve->Draw(shader, projection, view, model);
    }

    void BSplineController::GenerateCurve(float delta)
    {
        std::vector<Vector3> samples = m_bSpline->GetSamples(delta);

        std::vector<StaticMesh::Vertex> vertices(samples.size());
        std::vector<uint32> indices((samples.size() - 1) * 2);

        for (uint32 i = 0; i < samples.size() - 1; ++i)
        {
            indices[i * 2    ] = i;
            indices[i * 2 + 1] = i + 1;
            vertices[i] = StaticMesh::Vertex(samples[i]);
        }
        // Add missing vertices
        vertices[samples.size() - 1] = StaticMesh::Vertex(samples[samples.size() - 1]);

        Pointer<StaticMeshModel> curveModel = StaticMeshModel::Create(vertices, indices);

        static uint32 bSplineControllerId = 0;
        std::string instanceName = "Spline" + ToString(bSplineControllerId++);

        m_curve = NewPointer<StaticMeshInstance>(instanceName, curveModel);
        m_curve->SetMaterial(0, m_curveMaterial);
        m_curve->SetDrawMode(Renderer::EDrawMode::Lines);
    }

    void BSplineController::TickUpdate(const Pointer<Scene> &scene)
    {
        auto nNodes = static_cast<int32>(m_bSpline->GetNbPoints());
        auto nNodesInstance = static_cast<int32>(m_nodeInstances.size());

        int32 delta = std::abs(nNodes - nNodesInstance);

        if (delta > 0)
        {
            if (nNodes > nNodesInstance) // Add new instances
            {
                for (int32 i = 0; i < delta; ++i)
                {
                    static uint32 instanceId = 0;
                    std::string instanceName = "BSplineNode" + ToString(instanceId++);

                    auto index = static_cast<uint32>(nNodesInstance + i);
                    const Vector3& location = m_bSpline->GetPointAt(index);

                    auto instance = NewPointer<StaticMeshInstance>(instanceName, s_nodeModel);
                    instance->SetMaterial(0, s_nodeMaterial);
                    instance->SetWorldLocation(location);
                    instance->SetWorldScale({0.1f, 0.1f, 0.1f});
                    m_nodeInstances.push_back(instance);

                    scene->PushComponent(instance);
                }
            }
            else // Erase invalid instances
            {
                m_nodeInstances.erase(m_nodeInstances.begin() + delta, m_nodeInstances.end());
            }
        }

        for (uint32 i = 0; i < m_bSpline->GetNbPoints(); ++i)
        {
            const Vector3& location = m_nodeInstances[i]->GetWorldLocation();
            m_bSpline->SetPointAt(i, location);
        }
    }
}