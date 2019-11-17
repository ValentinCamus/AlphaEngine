#include "Nurbs.h"

namespace Alpha
{
    Vector3 Nurbs::Evaluate(float u)
    {
        std::vector<Vector3> pts; // The points which will influence the curve.
        std::vector<float> weights; // The weights which will influence the curve.

        uint32 offset = GetInterestPointsOffset(u);

        for (uint32 i = 0; i < BSpline::GetOrder(); ++i)
        {
            pts.push_back(BSpline::GetPointAt(i + offset));
            weights.push_back(m_weights.at(i + offset));
        }

        uint32 index = BSpline::GetOrder();
        for (uint32 j = 0; j < BSpline::GetOrder() - 1; ++j)
        {
            for (uint32 i = 0; i < index - 1; ++i)
            {
                float a = ((BSpline::GetKnotAt(offset + index + i) - u) /
                           (BSpline::GetKnotAt(offset + index + i) - BSpline::GetKnotAt(offset + 1 + i)));

                float b = ((u - BSpline::GetKnotAt(offset + 1 + i)) /
                           (BSpline::GetKnotAt(offset + index + i) - BSpline::GetKnotAt(offset + 1 + i)));

                pts[i] = a * pts[i] * weights[i] + b * pts[i + 1] * weights[i + 1];
                weights[i] = a * weights[i] + b * weights[i + 1];
            }
            --index;
            ++offset;
        }

        return pts[0] / weights[0];
    }

    bool Nurbs::SetNbPoints(uint32 size)
    {
        if (size < BSpline::GetOrder())
        {
            Logger::Error("Invalid number of points: {0}, this value doesn't respect the property:", size);
            Logger::Error("To define a curve, you need at least <order> control points");
            return false;
        }

        m_weights.resize(size);
        BSpline::SetNbPoints(size);

        return true;
    }

    void Nurbs::MakeUniformWeights()
    {
        for (float& weight : m_weights) weight = 1.0f;
    }
}