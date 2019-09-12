#include "OpenGLTexture.h"

#include <Alpha/ThirdParty/stb/stb_image.h>

namespace Alpha
{

    void OpenGLTexture2D::Init(const std::string &path)
    {
        stbi_set_flip_vertically_on_load(true);

        Logger::Info("Loading texture: {0}", path);
        m_localBuffer = stbi_load(path.c_str(), &m_width, &m_height, &m_bitsPerPixel, 4);
        ALPHA_ASSERT(m_localBuffer, "Texture::Init : file not found");

        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_2D, m_id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // With: The second argument: No multi level texture
        // With: The six argument: No border
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer);
        glBindTexture(GL_TEXTURE_2D, 0);

        stbi_image_free(m_localBuffer);
    }

    void OpenGLTexture2D::Bind(int32 slot)
    {
        m_slot = slot;
        glActiveTexture(GL_TEXTURE0 + (GLenum)slot);
        glBindTexture(GL_TEXTURE_2D, m_id);
    }
}
