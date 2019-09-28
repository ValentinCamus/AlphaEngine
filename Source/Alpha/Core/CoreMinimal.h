#pragma once

// Default STL includes
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <iostream>

#define ALPHA_ENABLE_CHECK
#define ALPHA_ENABLE_ASSERT
#include <Alpha/Core/CoreMacros.h>
#include <Alpha/Core/Logger.h>
#include <Alpha/Core/Types.h>
#include <Alpha/Core/Utils.h>
#include <Alpha/Core/Math/Math.h>
#include <Alpha/Core/Library.h>
#include <Alpha/Core/BSpline/BSpline.h>
#include <Alpha/Core/BSpline/TensorProduct.h>

namespace Alpha
{
    class Core
    {
    public:
        static inline void Init()
        {
            Alpha::Logger::Info("Initializing: AlphaEngine Core...");
            Alpha::SetAtExitCallback();
            Alpha::Logger::SetLogLevel(Alpha::ELogLevel::Debug);
            Random::SetSeed();
            Alpha::Logger::Info("AlphaEngine Core initialized");
        }
    };
}