#include "OpenGLTexture.h"

#include <Alpha/Engine/Loader/ImageLoader.h>

namespace Alpha
{

    void OpenGLTexture2D::Init(const std::string &path)
    {
        RawImage image = ImageLoader::Load(path);
        m_width = image.width;
        m_height = image.height;
        m_bitsPerPixel = image.bitsPerPixel;

        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_2D, m_id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // With: The second argument: No multi level texture
        // With: The six argument: No border
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.pixels);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void OpenGLTexture2D::Bind(int32 slot)
    {
        m_slot = slot;
        glActiveTexture(GL_TEXTURE0 + (GLenum)slot);
        glBindTexture(GL_TEXTURE_2D, m_id);
    }
}
