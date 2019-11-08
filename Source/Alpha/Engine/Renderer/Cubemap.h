#pragma once

#include <Alpha/Core/CoreMinimal.h>

namespace Alpha
{
    class Cubemap
    {
    public:
        static Pointer<Cubemap> Create(const std::vector<std::string>& paths);

    public:
        virtual ~Cubemap() = default;

        virtual void Bind(int32 slot) = 0;
        virtual void Unbind() = 0;

        virtual uint32 GetId() const = 0;
        virtual int32 GetSlot() const = 0;
    };
}

