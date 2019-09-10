#include "LayerStack.h"

namespace Alpha
{
    LayerStack::LayerStack(const std::string &name) : m_name(name)
    {
        Logger::Info("Creating new layer stack: {0}", name);
    }

    LayerStack::~LayerStack() { m_layers.clear(); }

    void LayerStack::PushLayer(const Pointer<Layer>& layer)
    {
        m_layers.emplace(m_layers.begin() + m_layerInsertIndex, layer);
        ++m_layerInsertIndex;
        layer->OnAttach();

        Logger::Info("{0}: Pushing new layer: {1}", GetName(), layer->GetName());
    }

    void LayerStack::PopLayer(const Pointer<Layer>& layer)
    {
        auto it = std::find(m_layers.begin(), m_layers.begin() + m_layerInsertIndex, layer);
        if (it != m_layers.begin() + m_layerInsertIndex)
        {
            layer->OnDetach();
            m_layers.erase(it);
            --m_layerInsertIndex;
        }
        else
        {
            Logger::Warn("{0}: Trying to pop an invalid layer: {1}", GetName(), layer->GetName());
        }
    }
}
