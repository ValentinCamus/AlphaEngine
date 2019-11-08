#include "ImageLoader.h"

#include <Alpha/ThirdParty/stb/stb_image.h>

namespace Alpha
{

    Pointer<RawImage> ImageLoader::Load(const std::string& filepath,
                                        bool bFlipYAxis,
                                        ImageLoader::EFormat nbChannels)
    {
        stbi_set_flip_vertically_on_load(bFlipYAxis);

        Pointer<RawImage> image = NewPointer<RawImage>();

        Logger::Info("Loading image \"{0}\"...", filepath);

        image->pixels = stbi_load(filepath.c_str(),
                                  &image->width,
                                  &image->height,
                                  &image->bitsPerPixel,
                                  static_cast<int32>(nbChannels));

        ALPHA_ASSERT(image->pixels, "ImageLoader::Load: Image not found");
        Logger::Info("Image \"{0}\" was correctly loaded", filepath);

        return image;
    }
}