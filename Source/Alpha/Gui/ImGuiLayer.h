#pragma once

#include <Alpha/ThirdParty/imgui/imgui.h>

#include <Alpha/Layer/Layer.h>

#include <Alpha/Event/ApplicationEvent.h>
#include <Alpha/Event/KeyEvent.h>
#include <Alpha/Event/MouseEvent.h>

namespace Alpha
{
    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer(const std::string& name = "ImGuiLayer");
        ~ImGuiLayer() override = default;

        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate() override;

        static void Begin();
        static void End();

        virtual void OnImGuiRender() = 0;

    private:
        float m_time = 0.0f;
};

}