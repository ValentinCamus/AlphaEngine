#pragma once


#include <random>
#include <Alpha/Core/Types.h>

namespace Alpha
{
    class Random
    {
    public:
        static void SetSeed();
        static void SetSeed(int32 seed);

        static int32 GetInt(int32 min = 0, int32 max = 100);
        static float GetFloat(float min = 0.0f, float max = 1.0f);
    };
}