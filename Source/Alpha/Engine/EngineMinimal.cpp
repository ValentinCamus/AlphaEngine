#include "EngineMinimal.h"

namespace Alpha
{
    Pointer<Library<Pointer<Scene>>> Engine::s_scenes = nullptr;
    Pointer<Library<Pointer<Texture2D>>> Engine::s_textures = nullptr;
    Pointer<Library<Pointer<Material>>> Engine::s_materials = nullptr;
    Pointer<Library<Pointer<Shader>>> Engine::s_shaders = nullptr;
    Pointer<Library<Pointer<StaticMeshModel>>> Engine::s_models = nullptr;

    void Engine::Init()
    {
        Logger::Info("Initializing: AlphaEngine Engine...");

        s_scenes = NewPointer<Library<Pointer<Scene>>>();
        s_textures = NewPointer<Library<Pointer<Texture2D>>>();
        s_materials = NewPointer<Library<Pointer<Material>>>();
        s_shaders = NewPointer<Library<Pointer<Shader>>>();
        s_models = NewPointer<Library<Pointer<StaticMeshModel>>>();

        Renderer::Init();

        Logger::Info("AlphaEngine Engine initialized");
    }
}

