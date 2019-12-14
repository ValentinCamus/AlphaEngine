#pragma once

#include <Alpha/Core/CoreMinimal.h>

#include <Alpha/Engine/BSpline/BSpline.h>
#include <Alpha/Engine/Renderer/Shader.h>
#include <Alpha/Engine/Mesh/StaticMeshInstance.h>

#include <Alpha/Engine/Scene/Scene.h>

namespace Alpha
{
    class BSplineController
    {
    public:

        explicit inline BSplineController() { Init(); }

        void Init();

        inline const Pointer<BSpline>& GetBSpline() const { return m_bSpline; }
        inline void SetBSpline(const Pointer<BSpline>& spline) { m_bSpline = spline; }

        void GenerateCurve(float delta = 0.1f);

        inline bool IsBSplineCurveValid() const { return m_curve != nullptr; }

        inline const Pointer<Material>& GetCurveMaterial() const { return m_curveMaterial; }
        inline void SetCurveMaterial(const Pointer<Material>& material) { m_curveMaterial = material; }

        inline void BindMaterials() const { Renderer::GetDrawOptions()->bUseMaterial = true; }

        inline void UnbindMaterials() const { Renderer::GetDrawOptions()->bUseMaterial = false; }

        void TickUpdate(const Pointer<Scene> &scene);

        /// Draw the entity.
        /// The model matrix will be set with the instance's transform.
        void Draw(const Pointer<Shader>& shader,
                  const Matrix4x4 * projection = nullptr,
                  const Matrix4x4 * view = nullptr) const;

        /// Draw the entity.
        void Draw(const Pointer<Shader>& shader,
                  const Matrix4x4 * projection,
                  const Matrix4x4 * view,
                  const Matrix4x4 * model) const;

    private:
        Pointer<BSpline> m_bSpline = nullptr;

        Pointer<Material> m_curveMaterial = nullptr;
        Pointer<StaticMeshInstance> m_curve = nullptr;

        std::vector<Pointer<StaticMeshInstance>> m_nodeInstances;

        static Pointer<Material> s_nodeMaterial;
        static Pointer<StaticMeshModel> s_nodeModel;
    };
}
