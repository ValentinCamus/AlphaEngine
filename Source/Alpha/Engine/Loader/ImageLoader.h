#pragma once

#include <Alpha/Core/CoreMinimal.h>

namespace Alpha
{
    class ImageLoader
    {
    public:
        /// Load an image from the disk.
        static Pointer<RawImage> Load(const std::string& filepath);
    };
}
