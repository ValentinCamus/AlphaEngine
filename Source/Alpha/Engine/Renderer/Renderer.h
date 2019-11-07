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
        enum class EDrawMode
        {
            Points,
            Lines,
            Triangles
        };

        struct DrawOptions
        {
            EDrawMode drawMode = EDrawMode::Triangles;
            bool bUseMaterial = false;
            bool bUseModelMatrix = false;
            bool bUseViewMatrix = false;
            bool bUseProjectionMatrix = false;

            inline void Reset()
            {
                drawMode = EDrawMode::Triangles;
                bUseMaterial = false;
                bUseModelMatrix = false;
                bUseViewMatrix = false;
                bUseProjectionMatrix = false;
            }
        };

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

        static inline const Pointer<DrawOptions>& GetDrawOptions() { return s_options; }

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

        static Pointer<DrawOptions> s_options;
    };
}

