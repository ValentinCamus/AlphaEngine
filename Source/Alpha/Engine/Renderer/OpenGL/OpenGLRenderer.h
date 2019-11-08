#pragma once

#include <Alpha/Engine/Renderer/Renderer.h>

#include <Alpha/Core/CoreMinimal.h>

#include <Alpha/ThirdParty/glad/glad.h>

#include <Alpha/Engine/Renderer/OpenGL/OpenGLTools.h>

namespace Alpha
{
    class OpenGLRendererAPI : public Renderer
    {
    public:
        inline OpenGLRendererAPI() { Logger::Info("(Renderer) Use OpenGL rendering API"); }

    protected:
        void InitImpl() override;

        void CreateContextImpl() override;

        void SetClearColorImpl(const Color4& color) override;

        void ClearImpl() override;

        void EnableDepthMaskImpl() override;

        void DisableDepthMaskImpl() override;
    };
}