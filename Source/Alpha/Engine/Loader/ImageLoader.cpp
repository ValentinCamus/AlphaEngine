#include "ImageLoader.h"

#include <Alpha/ThirdParty/stb/stb_image.h>

namespace Alpha
{

    Pointer<RawImage> ImageLoader::Load(const std::string &filepath)
    {
        stbi_set_flip_vertically_on_load(true);

        Pointer<RawImage> image = NewPointer<RawImage>();

        Logger::Info("Loading image \"{0}\"...", filepath);
        image->pixels = stbi_load(filepath.c_str(), &image->width, &image->height, &image->bitsPerPixel, 4);
        ALPHA_ASSERT(image->pixels, "ImageLoader::Load: Image not found");
        Logger::Info("Image \"{0}\" was correctly loaded", filepath);

        return image;
    }
}