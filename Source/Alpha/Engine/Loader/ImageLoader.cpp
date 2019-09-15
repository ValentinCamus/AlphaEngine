#include "ImageLoader.h"

#include <Alpha/ThirdParty/stb/stb_image.h>

namespace Alpha
{

    RawImage ImageLoader::Load(const std::string &filepath)
    {
        stbi_set_flip_vertically_on_load(true);

        RawImage image;

        Logger::Info("Loading image \"{0}\"...", filepath);
        image.pixels = stbi_load(filepath.c_str(), &image.width, &image.height, &image.bitsPerPixel, 4);
        ALPHA_ASSERT(image.pixels, "ImageLoader::Load: Image not found");
        Logger::Info("Image \"{0}\" loaded correctly", filepath);
        return image;
    }
}