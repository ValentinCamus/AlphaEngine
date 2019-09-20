#include "BSpline.h"

namespace Alpha
{
    Vector3 BSpline::GetSplinePointAt(float u)
    {
        uint32 l = m_order;
        std::vector<Vector3> pts;
        uint32 offset = GetInterestPointsOffset(u);

        pts.reserve(m_order);
        for (uint32 i = 0; i < m_order; ++i) pts[i] = m_controlPoints[i + offset];

        for (uint32 j = 0; j < m_order - 1; ++j)
        {
            for (uint32 i = 0; i < l - 1; ++i)
            {
                pts[i] = ((m_nodes[offset + l + i] - u) /
                          (m_nodes[offset + l + i] - m_nodes[offset + 1 + i])) * pts[i + 0]
                         +
                         ((u - m_nodes[offset + 1 + i]) /
                          (m_nodes[offset + l + i] - m_nodes[offset + 1 + i])) * pts[i + 1];

            }
            --l;
            ++offset;
        }

        return pts[0];
    }

    uint32 BSpline::GetInterestPointsOffset(float u)
    {
        uint32 offset = 0;
        uint32 i = m_order;

        while (m_nodes[i] < u)
        {
            ++offset;
            ++i;
        }

        return offset;
    }
}