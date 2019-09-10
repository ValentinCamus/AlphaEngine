#pragma once

#include <Alpha/Core/CoreMinimal.h>

namespace Alpha
{
	namespace Version
	{
		constexpr uint32 MAJOR    = 0;
		constexpr uint32 MINOR    = 0;
		constexpr uint32 REVISION = 1;

		inline std::string AsString()
		{
			return ToString(MAJOR) + "." + ToString(MINOR) + "." + ToString(REVISION);
		}
	}
}
