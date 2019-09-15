#pragma once

#include <Alpha/Core/CoreMinimal.h>

namespace Alpha
{
    enum class ERendererAPI
    {
        None,
        OpenGL
    };

    class Renderer
    {
    public:
        virtual ~Renderer() = default;

        static inline ERendererAPI GetRendererAPI() { return s_rendererAPI; }

        static void Init();

        static inline void CreateContext()
        {
            ALPHA_ASSERT(s_instance, "Invalid renderer!");
            s_instance->CreateContextImpl();
        }

        static inline void Clear()
        {
            ALPHA_ASSERT(s_instance, "Invalid renderer!");
            s_instance->ClearImpl();
        }

        static inline void SetClearColor(const Color4& color)
        {
            ALPHA_ASSERT(s_instance, "Invalid renderer!");
            s_instance->SetClearColorImpl(color);
        }

    protected:
        /// Initialize the renderer.
        virtual void InitImpl() = 0;

        /// Create the renderer context.
        virtual void CreateContextImpl() = 0;

        virtual void SetClearColorImpl(const Color4& color) = 0;

        virtual void ClearImpl() = 0;

    private:
        static Renderer* s_instance;
        static ERendererAPI s_rendererAPI;
    };
}

