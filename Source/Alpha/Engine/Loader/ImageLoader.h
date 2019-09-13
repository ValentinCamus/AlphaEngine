#pragma once

#include <Alpha/Core/CoreMinimal.h>

namespace Alpha
{
    class ImageLoader
    {
    public:
        /// Load an image from the disk.
        static RawImage Load(const std::string& filepath);
    };
}
