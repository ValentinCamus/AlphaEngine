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
            Vector3 ySample;

            for (uint32 j = 0; j < m_surface.height; ++j)
            {
                BSpline ySpline;
                ySpline.SetDegree(m_degree);
                ySpline.SetNbPoints(m_surface.height);

                for (uint32 k = 0; k < m_surface.height; ++k)
                    ySpline.SetPointAt(k, m_surface.GetPointAt(i, k));

                ySpline.ResetKnotsVector();

                ySample = ySpline.Evaluate(v);
            }

            xSpline.SetPointAt(i, ySample);
        }

        xSpline.ResetKnotsVector();

        return xSpline.Evaluate(u);
    }
}