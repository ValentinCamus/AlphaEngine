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
#include <Alpha/Core/Math.h>
#include <Alpha/Core/Collection.h>

namespace Alpha
{
    namespace Core
    {
        inline void Init()
        {
            Alpha::SetAtExitCallback();
            Alpha::Logger::SetLogLevel(Alpha::ELogLevel::Debug);

            Alpha::Logger::Info("AlphaEngine Core initialization [OK]");
        }
    }
}