#include "OpenGLFramebuffer.h"

namespace Alpha
{
    OpenGL::FramebufferTexture2D::FramebufferTexture2D(uint32 width, uint32 height)
        : m_width(width)
        , m_height(height)
    {
        Init(width, height);
    }

    void OpenGL::FramebufferTexture2D::Init(uint32 width, uint32 height)
    {
        GL_CHECK(glGenTextures(1, &m_id));
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_id));
        Resize(width, height);
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    }

    void OpenGL::FramebufferTexture2D::Bind(int32 slot)
    {
        m_slot = slot;
        //GL_CHECK(glViewport(0, 0, m_width, m_height));
        GL_CHECK(glActiveTexture(GL_TEXTURE0 + (GLenum)slot));
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_id));
    }

    void OpenGL::FramebufferTexture2D::Unbind()
    {
        m_slot = -1;
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
    }

    void OpenGL::FramebufferTexture2D::Resize(uint32 width, uint32 height)
    {
        m_width = width;
        m_height = height;

        GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
    }




    OpenGLFramebuffer::OpenGLFramebuffer(uint32 width, uint32 height)
            : m_width(width)
            , m_height(height)
    {
        Init(width, height);
    }

    void OpenGLFramebuffer::Resize(uint32 width, uint32 height)
    {
        m_width = width; m_height = height;

        GL_CHECK(glViewport(0, 0, width, height));
        GL_CHECK(glBindRenderbuffer(GL_RENDERBUFFER, m_rbo));
        GL_CHECK(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));

        m_texture->Resize(width, height);
    }

    void OpenGLFramebuffer::Bind()
    {
        if (!IsValid())
        {
            Logger::Warn("(OpenGLFramebuffer) Invalid fbo index - Nothing happened");
            return;
        }

        GL_CHECK(glViewport(0, 0, m_width, m_height));
        GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo));

    }

    void OpenGLFramebuffer::Init(uint32 width, uint32 height)
    {
        GL_CHECK(glGenFramebuffers(1, &m_fbo));
        GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo));

        // Create a color attachment texture
        m_texture = NewPointer<OpenGL::FramebufferTexture2D>(width, height);
        GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture->GetId(), 0));

        // Create a renderbuffer object for depth and stencil attachment
        GL_CHECK(glGenRenderbuffers(1, &m_rbo));
        GL_CHECK(glBindRenderbuffer(GL_RENDERBUFFER, m_rbo));
        // Use a single renderbuffer object for both a depth AND stencil buffer.
        GL_CHECK(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));
        // Attach the render buffer.
        GL_CHECK(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo));

        // Now that we actually created the framebuffer and added all attachments,
        // we want to check if it is actually complete now.
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            Logger::Error("OpenGLFramebuffer: Framebuffer is not complete!");
            Alpha::ForceQuit(EXIT_FAILURE);
        }

        GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }
}