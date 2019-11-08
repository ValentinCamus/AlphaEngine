#pragma once

#include <Alpha/Core/CoreMinimal.h>

namespace Alpha
{
    class ImageLoader
    {
    public:
        enum class EFormat
        {
            Default     = 0, // only used for desired_channels
            Grey        = 1,
            GreyAlpha   = 2,
            RGB         = 3,
            RGBA        = 4
        };

    public:
        /// Load an image from the disk.
        static Pointer<RawImage> Load(const std::string& filepath,
                                      bool bFlipYAxis = true,
                                      EFormat nbChannels = EFormat::RGBA);
    };
}
