#pragma once

#include <Alpha/Core/CoreMinimal.h>

namespace Alpha
{
    class Texture2D
    {
    public:
        static Pointer<Texture2D> Create(const std::string& path);

    public:
        virtual ~Texture2D() = default;

        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;

        virtual void Bind(int32 slot) = 0;
        virtual void Unbind() = 0;

        virtual uint32 GetId() const = 0;
        virtual int32 GetSlot() const = 0;
    };
}