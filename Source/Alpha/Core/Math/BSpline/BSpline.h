#pragma once

#include <Alpha/Core/CoreMinimal.h>

namespace Alpha
{
    class BSpline
    {
    public:
        Vector3 GetSplinePointAt(float u);

        void ResetKnotsVector();

        void SetDegree(uint32 degree);

        inline const Vector3& GetPointAt(uint32 idx) const { return m_points[idx]; }
        inline void SetPointAt(uint32 idx, const Vector3& pts) { m_points[idx] = pts; }

        inline float GetKnotAt(uint32 idx) const { return m_knots[idx]; }
        bool SetKnotAt(uint32 idx, float value);

        inline uint32 GetNbPoints() const { return m_points.size(); }
        bool SetNbPoints(uint32 size);

        inline uint32 GetNbKnots() const { return m_knots.size(); }

        inline Vector2 GetDomainOfDefinition() const { return Vector2(m_order, m_points.size()); }

        inline uint32 GetDegree() const { return m_degree; }
        inline uint32 GetOrder() const { return m_order; }

        std::vector<Vector3> GetSamples(float delta);

    private:
        uint32 GetInterestPointsOffset(float u);

        void ResizeKnotsVector(uint32 size);

    private:
        /// The degree of the spline, also known as m.
        uint32 m_degree = 0;

        /// The order of the spline, also known as k.
        /// @note: order = degree + 1.
        uint32 m_order = 0;

        /// The control points of the BSpline.
        /// @note: To define a curve, you need at least order control points.
        std::vector<Vector3> m_points;

        /// A spline of order n is a piecewise polynomial function of degree n-1 in a variable x.
        /// The values of x where the pieces of polynomial meet are known as knots.
        /// Number of knots = order + number of control points.
        /// @note: Sorted into non-decreasing order.
        /// @note: Domain of definition of the curve: u in [knots[order - 1], knots[degree - 1]]
        std::vector<float> m_knots;
    };
}