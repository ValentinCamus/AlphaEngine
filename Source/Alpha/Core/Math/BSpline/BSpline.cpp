#include "BSpline.h"

namespace Alpha
{
    Vector3 BSpline::GetSplinePointAt(float u)
    {
        std::vector<Vector3> pts; // The points which will influence the curve.

        uint32 offset = GetInterestPointsOffset(u);

        for (uint32 i = 0; i < m_order; ++i) pts.push_back(m_points[i + offset]);

        uint32 index = m_order;
        for (uint32 j = 0; j < m_order - 1; ++j)
        {
            for (uint32 i = 0; i < index - 1; ++i)
            {
                Vector3 a = ((m_knots[offset + index + i] - u) /
                             (m_knots[offset + index + i] - m_knots[offset + 1 + i])) * pts[i + 0];

                Vector3 b = ((u - m_knots[offset + 1 + i]) /
                             (m_knots[offset + index + i] - m_knots[offset + 1 + i])) * pts[i + 1];

                pts[i] = a + b;
            }
            --index;
            ++offset;
        }

        return pts[0];
    }

    uint32 BSpline::GetInterestPointsOffset(float u)
    {
        uint32 offset = 0;
        uint32 i = m_order;

        while (m_knots[i] < u)
        {
            ++offset;
            ++i;
        }
        return offset;
    }

    void BSpline::SetDegree(uint32 degree)
    {
        bool bIsIncreasing = degree > m_degree;

        m_degree = degree;
        m_order = degree + 1;

        if (m_points.size() < m_order) m_points.resize(m_order);

        ResizeKnotsVector(m_order + m_points.size());
    }

    bool BSpline::SetKnotAt(uint32 idx, float value)
    {
        bool bIsValid = true;
        if (idx > 0) bIsValid = m_knots[idx - 1] <= value;

        if (!bIsValid)
        {
            Logger::Error("Invalid knot value: {0}, this value doesn't respect the property:", value);
            Logger::Error("A knots vector is sorted into non-decreasing order");
            return false;
        }
        m_knots[idx] = value;
        return true;
    }

    bool BSpline::SetNbPoints(uint32 size)
    {
        if (size < m_order)
        {
            Logger::Error("Invalid number of points: {0}, this value doesn't respect the property:", size);
            Logger::Error("To define a curve, you need at least <order> control points");
            return false;
        }

        m_points.resize(size);
        ResizeKnotsVector(m_order + m_points.size());
        return true;
    }

    void BSpline::ResizeKnotsVector(uint32 size)
    {
        uint32 knotsOldSize = m_knots.size();
        uint32 knotsNewSize = size;

        m_knots.resize(size);
        ResetKnotsVector();
    }

    void BSpline::ResetKnotsVector()
    {
        for (uint32 i = 0; i < GetNbKnots(); ++i) m_knots[i] = float(i);
    }

    std::vector<Vector3> BSpline::GetSamples(float delta)
    {
        std::vector<Vector3> sampledSplinePoints;

        float u = GetDomainOfDefinition().x;

        while (u <= GetDomainOfDefinition().y)
        {
            Vector3 p = GetSplinePointAt(u);
            sampledSplinePoints.push_back(p);

            u += delta;
        }

        return sampledSplinePoints;
    }
}