#pragma once

#include <Alpha/Core/CoreMinimal.h>

#include <Alpha/Engine/BSpline/BSpline.h>

namespace Alpha
{
    class Nurbs : public BSpline
    {
    public:
        Vector3 Evaluate(float u) override;

        bool SetNbPoints(uint32 size) override;

        inline float GetWeightAt(uint32 index) const { return m_weights.at(index); }
        inline void SetWeightAt(uint32 index, float weight) { m_weights.at(index) = weight; }

        void MakeUniformWeights();

    private:
        std::vector<float> m_weights;
    };
}