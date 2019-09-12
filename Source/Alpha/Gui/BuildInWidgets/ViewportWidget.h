#pragma once

#include <Alpha/Core/CoreMinimal.h>

#include <Alpha/ThirdParty/imgui/imgui.h>
#include <Alpha/Gui/ImGuiWidget.h>

#include <Alpha/Engine/Renderer/Framebuffer.h>

#include <utility>

namespace Alpha
{
    class ViewportWidget : public ImGuiWidget
    {
    public:
        explicit ViewportWidget(const std::string &name) : ImGuiWidget(name) {}

        void Render() override;

        inline const Pointer<Framebuffer>& GetFramebuffer() const { return m_framebuffer; }
        inline void SetFramebuffer(const Pointer<Framebuffer>& fb) { m_framebuffer = fb; }

    private:
        Pointer<Framebuffer> m_framebuffer;
    };
}