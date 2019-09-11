#pragma once

#include <Alpha/Core/CoreMinimal.h>

#include <Alpha/Engine/Renderer/Framebuffer.h>
#include <Alpha/Engine/Renderer/Texture.h>
#include <Alpha/Engine/Renderer/OpenGL/OpenGLRenderer.h>

namespace Alpha
{
    namespace OpenGL
    {
        class FramebufferTexture2D : public Texture2D
        {
        public:
            FramebufferTexture2D(uint32 width, uint32 height) { Init(width, height); }
            ~FramebufferTexture2D() override = default;

            inline void Init(uint32 width, uint32 height)
            {
                glGenTextures(1, &m_id);
                glBindTexture(GL_TEXTURE_2D, m_id);
                Resize(width, height);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            }

            inline uint32_t GetWidth() const override { return m_width; }
            inline uint32_t GetHeight() const override { return m_height; }

            inline void Bind(uint32 slot) override
            {
                m_slot = slot;
                glActiveTexture(GL_TEXTURE0 + (GLenum)slot);
                glBindTexture(GL_TEXTURE_2D, m_id);
            }

            inline void Unbind() override
            {
                m_slot = 0;
                glBindTexture(GL_TEXTURE_2D, 0);
            }

            inline void Resize(uint32 width, uint32 height)
            {
                m_width = width; m_height = height;
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
            }

            inline uint32 GetId() const override { return m_id; }

        private:
            uint32 m_width = 0;
            uint32 m_height = 0;

            uint32 m_id = 0;
            uint32 m_slot = 0;
        };
    }

    class OpenGLFramebuffer : public Framebuffer
    {
    public:
         OpenGLFramebuffer(uint32 width, uint32 height) { Init(width, height); }
         ~OpenGLFramebuffer() override { Destroy(); }

         inline bool IsValid() override { return m_fbo > 0; }

         inline void Bind() override
         {
             if (!IsValid()) Logger::Warn("(OpenGLFramebuffer) Invalid fbo index");
             else glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
         }

         inline void Unbind() override { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

         inline uint32 GetWidth() override { return m_width; }
         inline uint32 GetHeight() override { return m_height; }

         inline void Resize(uint32 width, uint32 height) override
         {
             m_width = width; m_height = height;
             m_texture->Resize(width, height);

             glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
             glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
         }

         inline Pointer<Texture2D> GetTexture() override { return m_texture; }

    private:
        inline void Init(uint32 width, uint32 height)
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

        inline void Destroy() {}

    private:
        uint32 m_fbo = 0;
        uint32 m_rbo = 0;

        uint32 m_width = 0;
        uint32 m_height = 0;

        Pointer<OpenGL::FramebufferTexture2D> m_texture = nullptr;
    };
}