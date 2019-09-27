#pragma once

#include <Alpha/Core/CoreMinimal.h>
#include <Alpha/Core/Math/BSpline/BSpline.h>

namespace Alpha
{
    class TensorProduct
    {
    public:
        struct Surface
        {
            uint32 width;
            uint32 height;

            std::vector<Vector3> points;

            inline void Resize(uint32 w, uint32 h)
            {
                width = w;
                height = h;
                points.resize(w * h);
            }

            inline const Vector3& GetPointAt(uint32 x, uint32 y) const { return points.at(x + width * y); }
            inline void SetPointAt(uint32 x, uint32 y, const Vector3& pts) { points.at(x + width * y) = pts; }
        };

    public:
        Vector3 Evaluate(float u, float v) const;

        inline uint32 GetDegree() const { return m_degree; }
        inline void SetDegree(uint32 degree) { m_degree = degree; }

        inline void Resize(uint32 w, uint32 h) { m_surface.Resize(w, h); }
        inline const Vector3& GetPointAt(uint32 x, uint32 y) const { return m_surface.GetPointAt(x, y); }
        inline void SetPointAt(uint32 x, uint32 y, const Vector3& p) { m_surface.SetPointAt(x, y, p); }

        inline Vector2 GetValidRangeWidth() const { return Vector2(m_degree + 1, m_surface.width); }
        inline Vector2 GetValidRangeHeight() const { return Vector2(m_degree + 1, m_surface.height); }

        inline const Surface& GetSurface() const { return m_surface; }

    private:
        uint32 m_degree = 3;

        Surface m_surface;
    };
}