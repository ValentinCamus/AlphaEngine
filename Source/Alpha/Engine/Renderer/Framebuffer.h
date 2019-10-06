#pragma once

#include <Alpha/Core/CoreMinimal.h>

#include <Alpha/Engine/Renderer/Texture.h>

namespace Alpha
{
    class Framebuffer
    {
    public:
        static Pointer<Framebuffer> Create(uint32 width, uint32 height);

    public:
        virtual ~Framebuffer() = default;

        virtual bool IsValid() = 0;

        virtual void Bind() = 0;
        virtual void Unbind() = 0;

        virtual uint32 GetWidth() = 0;
        virtual uint32 GetHeight() = 0;

		virtual float GetAspectRatio() = 0;

        virtual void Resize(uint32 width, uint32 height) = 0;

        virtual Pointer<Texture2D> GetTexture() = 0;
    };


}