#pragma once

#include <Alpha/Core/CoreMinimal.h>

#include <Alpha/Engine/Renderer/Framebuffer.h>
#include <Alpha/Engine/Renderer/Texture.h>

#include <Alpha/Engine/Renderer/OpenGL/OpenGLTools.h>
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

            void Init(uint32 width, uint32 height);

            inline uint32_t GetWidth() const override { return m_width; }
            inline uint32_t GetHeight() const override { return m_height; }

            void Bind(int32 slot) override;

            void Unbind() override;

            void Resize(uint32 width, uint32 height);

            inline uint32 GetId() const override { return m_id; }
            inline int32 GetSlot() const override { return m_slot; }
        private:
            uint32 m_width = 0;
            uint32 m_height = 0;

            uint32 m_id = 0;
            int32 m_slot = -1;
        };
    }

    class OpenGLFramebuffer : public Framebuffer
    {
    public:
         OpenGLFramebuffer(uint32 width, uint32 height) { Init(width, height); }
         ~OpenGLFramebuffer() override { Destroy(); }

         inline bool IsValid() override { return m_fbo > 0; }

         void Bind() override;

         inline void Unbind() override { GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0)); }

         inline uint32 GetWidth() override { return m_width; }
         inline uint32 GetHeight() override { return m_height; }

		 inline float GetAspectRatio() override { return float(m_width) / float(m_height); }

         void Resize(uint32 width, uint32 height) override;

         inline Pointer<Texture2D> GetTexture() override { return m_texture; }

    private:
        void Init(uint32 width, uint32 height);

        inline void Destroy() { GL_CHECK(glDeleteFramebuffers(1, &m_fbo)); }

    private:
        uint32 m_fbo = 0;
        uint32 m_rbo = 0;

        uint32 m_width = 0;
        uint32 m_height = 0;

        Pointer<OpenGL::FramebufferTexture2D> m_texture = nullptr;
    };
}