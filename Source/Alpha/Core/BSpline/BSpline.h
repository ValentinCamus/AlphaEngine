#pragma once

#include <Alpha/Core/CoreMinimal.h>

namespace Alpha
{
    class BSpline
    {
    public:
        explicit BSpline(uint32 order,
                         const std::vector<Vector3>& controlPoints,
                         const std::vector<float>& nodes)
                :   m_order(order)
                ,   m_controlPoints(controlPoints)
                ,   m_nodes(nodes) {}

        Vector3 GetSplinePointAt(float u);

    private:
        uint32 GetInterestPointsOffset(float u);

    private:
        uint32 m_order;

        std::vector<Vector3> m_controlPoints;

        std::vector<float> m_nodes;
    };
}