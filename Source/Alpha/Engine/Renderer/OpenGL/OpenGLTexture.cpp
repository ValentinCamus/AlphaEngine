#include "OpenGLTexture.h"

#include <Alpha/Engine/Loader/ImageLoader.h>
#include <Alpha/ThirdParty/stb/stb_image.h>

namespace Alpha
{

    void OpenGLTexture2D::Init(const std::string &path)
    {
        /*
        RawImage image = ImageLoader::Load(path);
        m_width = image.width;
        m_height = image.height;
        m_bitsPerPixel = image.bitsPerPixel;

        GL_CHECK(glGenTextures(1, &m_id));
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_id));

        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

        // With: The second argument: No multi level texture
        // With: The six argument: No border
        GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.pixels));
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
        */

        stbi_set_flip_vertically_on_load(true);

        Logger::Info("Loading texture: {0}", path);
        m_localBuffer = stbi_load(path.c_str(), &m_width, &m_height, &m_bitsPerPixel, 4);
        ALPHA_ASSERT(m_localBuffer, "Texture::Init : file not found");

        GL_CHECK(glGenTextures(1, &m_id));
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_id));

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // With: The second argument: No multi level texture
        // With: The six argument: No border
        GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer));
        GL_CHECK(glGenerateMipmap(GL_TEXTURE_2D));
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));

        stbi_image_free(m_localBuffer);
    }

    void OpenGLTexture2D::Bind(int32 slot)
    {
        m_slot = slot;
        GL_CHECK(glActiveTexture(GL_TEXTURE0 + (GLenum)slot));
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_id));
    }
}
