#include "OpenGLDepthBuffer.h"

namespace Alpha
{
    OpenGL::DepthBufferTexture2D::DepthBufferTexture2D(uint32 width, uint32 height)
            : m_width(width)
            , m_height(height)
    {
        Init(width, height);
    }

    void OpenGL::DepthBufferTexture2D::Init(uint32 width, uint32 height)
    {
        constexpr float BORDER_COLOR[] = { 1.0, 1.0, 1.0, 1.0 };

        GL_CHECK(glGenTextures(1, &m_id));
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_id));
        Resize(width, height);

        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));

        GL_CHECK(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, BORDER_COLOR));

        Unbind();
    }

    void OpenGL::DepthBufferTexture2D::Resize(uint32 width, uint32 height)
    {
        constexpr GLenum FORMAT = GL_DEPTH_COMPONENT;

        m_width = width;
        m_height = height;

        GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, FORMAT, width, height, 0, FORMAT, GL_FLOAT, nullptr));
    }

    void OpenGL::DepthBufferTexture2D::Bind(int32 slot)
    {
        m_slot = slot;
        //GL_CHECK(glViewport(0, 0, m_width, m_height));
        GL_CHECK(glActiveTexture(GL_TEXTURE0 + (GLenum)slot));
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_id));
    }

    void OpenGL::DepthBufferTexture2D::Unbind()
    {
        m_slot = -1;
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
    }



    OpenGLDepthBuffer::OpenGLDepthBuffer(uint32 width, uint32 height)
            : m_width(width)
            , m_height(height)
    {
        Init(width, height);
    }

    void OpenGLDepthBuffer::Resize(uint32 width, uint32 height)
    {
        m_width = width; m_height = height;
        m_texture->Resize(width, height);
    }

    void OpenGLDepthBuffer::Bind()
    {
        if (!IsValid())
        {
            Logger::Warn("(OpenGLDepthBuffer) Invalid fbo index - Nothing happened");
            return;
        }

        GL_CHECK(glViewport(0, 0, m_width, m_height));
        GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo));
        GL_CHECK(glClear(GL_DEPTH_BUFFER_BIT));
    }

    void OpenGLDepthBuffer::Init(uint32 width, uint32 height)
    {
        constexpr GLenum ATTACHMENT = GL_DEPTH_ATTACHMENT;

        m_width = width;
        m_height = height;

        m_texture = NewPointer<OpenGL::DepthBufferTexture2D>(width, height);

        GL_CHECK(glGenFramebuffers(1, &m_fbo));
        GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo));

        GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, ATTACHMENT, GL_TEXTURE_2D, m_texture->GetId(), 0));

        GL_CHECK(glDrawBuffer(GL_NONE));
        GL_CHECK(glReadBuffer(GL_NONE));

        // Now that we actually created the framebuffer and added all attachments,
        // we want to check if it is actually complete now.
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            Logger::Error("OpenGLDepthBuffer: Depth Buffer is not complete!");
            Alpha::ForceQuit(EXIT_FAILURE);
        }

        GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }
}