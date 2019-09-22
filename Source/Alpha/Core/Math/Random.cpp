#include "Random.h"

#include <ctime>
#include <algorithm>
#include <functional>

namespace Alpha
{
    void Random::SetSeed()
    {
        srand(unsigned(time(nullptr)));
    }

    void Random::SetSeed(int32 seed)
    {
        srand(seed);
    }

    int32 Random::GetInt(int32 min, int32 max)
    {
        return (rand() % (max - min + 1)) + min;
    }

    float Random::GetFloat(float min, float max)
    {
        float scale = rand() / (float) RAND_MAX; // [0, 1.0]
        return min + scale * (max - min);        // [min, max]
    }
}

