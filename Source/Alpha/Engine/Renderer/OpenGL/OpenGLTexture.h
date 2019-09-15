#pragma once

#include <Alpha/Core/CoreMinimal.h>

#include <Alpha/Engine/Renderer/Texture.h>
#include <Alpha/Engine/Renderer/OpenGL/OpenGLTools.h>
#include <Alpha/Engine/Renderer/OpenGL/OpenGLRenderer.h>

namespace Alpha
{
    class OpenGLTexture2D : public Texture2D
    {
    public:
        /// Default constructor.
        OpenGLTexture2D() = default;

        /// Constructor.
        /// Load the texture from a file.
        explicit OpenGLTexture2D(const std::string& path) : m_filename(path) { Init(path); }

        /// Destructor.
        ~OpenGLTexture2D() override { GL_CHECK(glDeleteTextures(1, &m_id)); }

        /// Bind this texture.
        /// @slot: The texture slot, 1 slot per texture.
        void Bind(int32 slot) override;

        /// Unbind this texture.
        inline void Unbind() override { GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0)); }

    public:
        /// @getter: The id of this texture.
        inline uint32 GetId() const override { return m_id; }

        /// @getter: The slot of this texture (valid only if bound).
        inline int32 GetSlot() const override { return m_slot; }

        /// @getter: The width of the texture.
        inline uint32 GetWidth() const override { return m_width; }

        /// @getter: The height of the texture.
        inline uint32 GetHeight() const override { return m_height; }

        /// @getter: The number of bits per pixel.
        inline uint32 GetBitsPerPixel() const { return m_bitsPerPixel; }

    private:
        void Init(const std::string& path);

    private:
        /// The id of this texture.
        /// @note: unique.
        uint32 m_id = 0;

        /// The path to the texture's file (if loaded from texture file).
        std::string m_filename;

        /// The slot of this texture (valid only if bound).
        int32 m_slot = -1;

        /// The width of the texture.
        int32 m_width = 0;

        /// The height of the texture.
        int32 m_height = 0;

        /// The number of bits per pixel.
        int32 m_bitsPerPixel = 0;

        /// Where the texture is stored.
        uchar* m_localBuffer = nullptr;
    };

}

