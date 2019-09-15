#pragma once

#include <Alpha/Core/CoreMinimal.h>

#include <Alpha/Engine/Renderer/Renderer.h>

#include <Alpha/Engine/Light/Light.h>
#include <Alpha/Engine/Light/DirectionalLight.h>
#include <Alpha/Engine/Light/PointLight.h>
#include <Alpha/Engine/Light/SpotLight.h>


namespace Alpha
{
    /// Use to store all the engine's global variables and functions.
    class Engine
    {
    public:
        static void Init();

        static inline const Collection<Pointer<Light>>& GetLights() { return *s_lights; }

    private:
        static Collection<Pointer<Light>>* s_lights;
    };
}
