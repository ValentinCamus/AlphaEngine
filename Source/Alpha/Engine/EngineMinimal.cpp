#include "EngineMinimal.h"

namespace Alpha
{
    Collection<Pointer<Light>>* Engine::s_lights = nullptr;

    void Engine::Init()
    {
        Logger::Info("Initializing: AlphaEngine Engine...");
        Renderer::Init();

        s_lights = new Collection<Pointer<Light>>();

        Logger::Info("AlphaEngine Engine initialized");
    }
}

