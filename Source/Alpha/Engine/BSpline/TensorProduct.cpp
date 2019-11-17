#include "TensorProduct.h"

namespace Alpha
{

    Vector3 TensorProduct::Evaluate(float u, float v) const
    {
        BSpline xSpline;
        xSpline.SetDegree(m_degree);
        xSpline.SetNbPoints(m_surface.width);

        for (uint32 i = 0; i < m_surface.width; ++i)
        {
            BSpline ySpline;
            ySpline.SetDegree(m_degree);
            ySpline.SetNbPoints(m_surface.height);

            for (uint32 j = 0; j < m_surface.height; ++j)
                ySpline.SetPointAt(j, m_surface.GetPointAt(i, j));

            ySpline.ResetKnotsVector();

            xSpline.SetPointAt(i, ySpline.Evaluate(v));
        }

        xSpline.ResetKnotsVector();

        return xSpline.Evaluate(u);
    }

    std::vector<Vector3> TensorProduct::GetSamples(float deltaU, float deltaV)
    {
		std::vector<Vector3> samples;

		Vector2 xDef = GetValidRangeWidth();
		Vector2 yDef = GetValidRangeHeight();

		for (float u = xDef.x; u < xDef.y; u += deltaU)
		{
			for (float v = yDef.x; v < yDef.y; v += deltaV)
			{
				samples.push_back(Evaluate(u, v));
			}
		}

		return samples;
    }
}