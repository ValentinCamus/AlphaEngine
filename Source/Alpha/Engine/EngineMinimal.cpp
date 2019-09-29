#include "EngineMinimal.h"

namespace Alpha
{
    void Engine::Init()
    {
        Logger::Info("Initializing: AlphaEngine Engine...");
        Renderer::Init();
        Logger::Info("AlphaEngine Engine initialized");
    }
}

