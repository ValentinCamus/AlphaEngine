#pragma once

#include <Alpha/Core/CoreMinimal.h>

#include <Alpha/Engine/Renderer/Renderer.h>

#include <Alpha/Engine/Light/Light.h>
#include <Alpha/Engine/Light/DirectionalLight.h>
#include <Alpha/Engine/Light/PointLight.h>
#include <Alpha/Engine/Light/SpotLight.h>

#include <Alpha/Engine/Material/Material.h>

#include <Alpha/Engine/Mesh/StaticMeshEntity.h>

#include <Alpha/Engine/Camera/Camera.h>
#include <Alpha/Engine/Camera/EulerCamera.h>

#include <Alpha/Engine/Scene/Scene.h>

namespace Alpha
{
    /// Use to store all the engine's global variables and functions.
    class Engine
    {
    public:
        static void Init();

        static const Pointer<Library<Pointer<Scene>>>& GetSceneLibrary() { return s_scenes; }
        static const Pointer<Library<Pointer<Texture2D>>>& GetTexture2DLibrary() { return s_textures; }
        static const Pointer<Library<Pointer<Material>>>& GetMaterialLibrary() { return s_materials; }
        static const Pointer<Library<Pointer<Shader>>>& GetShaderLibrary() { return s_shaders; }
        static const Pointer<Library<Pointer<StaticMeshModel>>>& GetStaticMeshModelLibrary() { return s_models; }

    private:
        static Pointer<Library<Pointer<Scene>>> s_scenes;
        static Pointer<Library<Pointer<Texture2D>>> s_textures;
        static Pointer<Library<Pointer<Material>>> s_materials;
        static Pointer<Library<Pointer<Shader>>> s_shaders;
        static Pointer<Library<Pointer<StaticMeshModel>>> s_models;
    };
}
