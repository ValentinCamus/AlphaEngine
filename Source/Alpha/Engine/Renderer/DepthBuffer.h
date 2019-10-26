#pragma once

#include <Alpha/Core/CoreMinimal.h>
#include <Alpha/Engine/Renderer/Texture.h>
#include <Alpha/Engine/Renderer/Framebuffer.h>

namespace Alpha
{
    class DepthBuffer : public Framebuffer
    {
    public:
        static Pointer<DepthBuffer> Create(uint32 width, uint32 height);
    };
}