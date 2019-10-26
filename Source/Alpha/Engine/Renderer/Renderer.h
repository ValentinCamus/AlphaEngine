#pragma once

#include <Alpha/Core/CoreMinimal.h>

namespace Alpha
{
    enum class EDrawMode
    {
        Points,
        Lines,
        Triangles
    };

    enum class ERendererAPI
    {
        None,
        OpenGL
    };

    class Renderer
    {
    public:
        enum class EOption
        {
            None,
            DiscardMaterial,
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

        static inline void Enable(EOption option)
        {
            switch (option)
            {
                case EOption::None:
                    // Do nothing
                    break;
                case EOption::DiscardMaterial:
                    s_bDiscardMaterial = true;
                    break;
            }
        }

        static inline void Disable(EOption option)
        {
            switch (option)
            {
                case EOption::None:
                    // Do nothing
                    break;
                case EOption::DiscardMaterial:
                    s_bDiscardMaterial = false;
                    break;
            }
        }

        static inline bool IsEnable(EOption option)
        {
            switch (option)
            {
                case EOption::DiscardMaterial: return s_bDiscardMaterial;
                case EOption::None: break;
            }

            Logger::Warn("EOption::None cannot be enable, return false by default");
            return false;
        }

        static inline bool IsDisable(EOption option) { return !IsEnable(option); }

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

        static bool s_bDiscardMaterial;
    };
}

