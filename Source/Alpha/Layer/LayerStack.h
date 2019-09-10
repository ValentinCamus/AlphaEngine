#pragma once

#include <Alpha/Core/CoreMinimal.h>

#include <Alpha/Layer/Layer.h>

namespace Alpha
{
    class LayerStack
    {
        using LayerStackIterator = std::vector<Pointer<Layer>>::iterator;

    public:
        explicit LayerStack(const std::string& name);

        ~LayerStack();

        /// Push a new layer.
        void PushLayer(const Pointer<Layer>& layer);

        /// Pop a specified layer.
        void PopLayer(const Pointer<Layer>& layer);

        inline LayerStackIterator begin() { return m_layers.begin(); }
        inline LayerStackIterator end() { return m_layers.end(); }

        inline const std::string& GetName() const { return m_name; }
    private:
        std::string m_name;
        std::vector<Pointer<Layer>> m_layers;
        uint32 m_layerInsertIndex = 0;
    };
}
