#pragma once

#include <Alpha/Engine/Renderer/Renderer.h>

#include <Alpha/Core/CoreMinimal.h>

#include <Alpha/ThirdParty/glad/glad.h>

namespace Alpha
{
    class OpenGLRendererAPI : public Renderer
    {
    public:
        inline OpenGLRendererAPI() { Logger::Info("Renderer: Use OpenGL API"); }

    protected:
        void InitImpl() override;

        void CreateContextImpl() override;

        void SetClearColorImpl(const Color4& color) override;

        void ClearImpl() override;
    };
}