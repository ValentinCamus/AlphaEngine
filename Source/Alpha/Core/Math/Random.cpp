#include "Random.h"

#include <ctime>

namespace Alpha
{
    static std::mt19937 s_rng;

    void Random::SetSeed()
    {
        std::random_device dev;
        s_rng.seed(dev());
    }

    void Random::SetSeed(int32 seed)
    {
        s_rng.seed(seed);
    }

    int32 Random::GetInt(int32 min, int32 max)
    {
        std::uniform_int_distribution<>dist(min, max);
        return dist(s_rng);
    }

    float Random::GetFloat(float min, float max)
    {
        std::uniform_real_distribution<>dist(min, max);
        return float(dist(s_rng));
    }
}

