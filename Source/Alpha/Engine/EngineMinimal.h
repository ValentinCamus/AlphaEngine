#pragma once

#include <Alpha/Core/CoreMinimal.h>

#include <Alpha/Engine/Renderer/Renderer.h>

#include <Alpha/Engine/Renderer/DepthBuffer.h>

#include <Alpha/Engine/Light/Light.h>
#include <Alpha/Engine/Light/DirectionalLight.h>
#include <Alpha/Engine/Light/PointLight.h>
#include <Alpha/Engine/Light/SpotLight.h>

#include <Alpha/Engine/Material/Material.h>

#include <Alpha/Engine/Mesh/StaticMeshEntity.h>

#include <Alpha/Engine/Camera/Camera.h>
#include <Alpha/Engine/Camera/EulerCamera.h>

#include <Alpha/Engine/Scene/Scene.h>

#include <Alpha/Engine/GlobalStorage/GlobalStorage.h>

namespace Alpha
{
    /// Use to store all the engine's global variables and functions.
    class Engine
        {
        public:
            static void Init();
    };
}
