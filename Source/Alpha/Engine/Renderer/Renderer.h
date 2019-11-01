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
            DiscardModelMatrix,
            DiscardViewMatrix,
            DiscardProjectionMatrix,
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

                case EOption::DiscardModelMatrix:
                    s_bDiscardModelMatrix = true;
                    break;

                case EOption::DiscardViewMatrix:
                    s_bDiscardViewMatrix = true;
                    break;

                case EOption::DiscardProjectionMatrix:
                    s_bDiscardProjectionMatrix = true;
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

                case EOption::DiscardModelMatrix:
                    s_bDiscardModelMatrix = false;
                    break;

                case EOption::DiscardViewMatrix:
                    s_bDiscardViewMatrix = false;
                    break;

                case EOption::DiscardProjectionMatrix:
                    s_bDiscardProjectionMatrix = false;
                    break;
            }
        }

        static inline bool IsEnable(EOption option)
        {
            switch (option)
            {
                case EOption::DiscardMaterial: return s_bDiscardMaterial;
                case EOption::DiscardModelMatrix: return s_bDiscardModelMatrix;
                case EOption::DiscardViewMatrix: return s_bDiscardViewMatrix;
                case EOption::DiscardProjectionMatrix: return s_bDiscardProjectionMatrix;
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
        static bool s_bDiscardModelMatrix;
        static bool s_bDiscardViewMatrix;
        static bool s_bDiscardProjectionMatrix;
    };
}

