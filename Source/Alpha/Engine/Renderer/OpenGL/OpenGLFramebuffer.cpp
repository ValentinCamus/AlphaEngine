#include "OpenGLFramebuffer.h"

namespace Alpha
{
    void OpenGL::FramebufferTexture2D::Init(uint32 width, uint32 height)
    {
        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_2D, m_id);
        Resize(width, height);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    void OpenGL::FramebufferTexture2D::Bind(int32 slot)
    {
        m_slot = slot;
        glActiveTexture(GL_TEXTURE0 + (GLenum)slot);
        glBindTexture(GL_TEXTURE_2D, m_id);
    }

    void OpenGL::FramebufferTexture2D::Unbind()
    {
        m_slot = -1;
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void OpenGL::FramebufferTexture2D::Resize(uint32 width, uint32 height)
    {
        m_width = width; m_height = height;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    }





    void OpenGLFramebuffer::Resize(uint32 width, uint32 height)
    {
        m_width = width; m_height = height;
        m_texture->Resize(width, height);

        glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    }

    void OpenGLFramebuffer::Bind()
    {
        if (!IsValid()) Logger::Warn("(OpenGLFramebuffer) Invalid fbo index");
        else glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    }

    void OpenGLFramebuffer::Init(uint32 width, uint32 height)
    {
        m_width = width; m_height = height;

        glGenFramebuffers(1, &m_fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

        // Create a color attachment texture
        m_texture = NewPointer<OpenGL::FramebufferTexture2D>(width, height);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture->GetId(), 0);

        // Create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
        glGenRenderbuffers(1, &m_rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
        // Use a single renderbuffer object for both a depth AND stencil buffer.
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        // Attach the render buffer.
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

        // Now that we actually created the framebuffer and added all attachments,
        // we want to check if it is actually complete now.
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            Logger::Error("OpenGLFramebuffer: Framebuffer is not complete!");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}