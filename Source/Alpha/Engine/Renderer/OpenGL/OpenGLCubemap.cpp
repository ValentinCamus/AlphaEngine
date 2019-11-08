#include "OpenGLCubemap.h"

#include <Alpha/Engine/Loader/ImageLoader.h>
#include <Alpha/ThirdParty/stb/stb_image.h>

namespace Alpha
{
    void OpenGLCubemap::Init(const std::vector<std::string>& paths)
    {
        GL_CHECK(glGenTextures(1, &m_id));
        GL_CHECK(glBindTexture(GL_TEXTURE_CUBE_MAP, m_id));

        for(uint32 i = 0; i < paths.size(); i++)
        {
            const std::string& path = paths[i];
            Pointer<RawImage> image = ImageLoader::Load(path, false, ImageLoader::EFormat::Default);

            auto w = static_cast<uint32>(image->width);
            auto h = static_cast<uint32>(image->height);
            uchar* pixels = image->pixels;

            GLenum target = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;

            GL_CHECK(glTexImage2D(target, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels));
        }

        GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

        GL_CHECK(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
    }
}