#pragma once

#include <Alpha/Core/CoreMinimal.h>

#include <Alpha/Engine/Scene/Scene.h>
#include <Alpha/Engine/Renderer/Texture.h>
#include <Alpha/Engine/Material/Material.h>
#include <Alpha/Engine/Renderer/Shader.h>
#include <Alpha/Engine/Mesh/StaticMeshModel.h>

namespace Alpha
{
    class GlobalStorage
    {
    public:
		static inline Pointer<Scene>& GetScene(const std::string& name)
		{
			static Pointer<Scene> output = nullptr;
			ALPHA_ASSERT(s_scenes.Contains(name), "GlobalStorage::Get<Scene>: Invalid name");
			output = s_scenes.Get(name);

			return output;
		}

		static inline Pointer<Texture2D>& GetTexture2D(const std::string& name)
		{
			static Pointer<Texture2D> output = nullptr;
			ALPHA_ASSERT(s_textures.Contains(name), "GlobalStorage::Get<Texture2D>: Invalid name");
			output = s_textures.Get(name);

			return output;
		}

		static inline Pointer<Material>& GetMaterial(const std::string& name)
		{
			static Pointer<Material> output = nullptr;
			ALPHA_ASSERT(s_materials.Contains(name), "GlobalStorage::Get<Material>: Invalid name");
			output = s_materials.Get(name);

			return output;
		}

		static inline Pointer<Shader>& GetShader(const std::string& name)
		{
			static Pointer<Shader> output = nullptr;
			ALPHA_ASSERT(s_shaders.Contains(name), "GlobalStorage::Get<Shader>: Invalid name");
			output = s_shaders.Get(name);

			return output;
		}

		static inline Pointer<StaticMeshModel>& GetStaticMeshModel(const std::string& name)
		{
			static Pointer<StaticMeshModel> output = nullptr;
			ALPHA_ASSERT(s_models.Contains(name), "GlobalStorage::Get<StaticMeshModel>: Invalid name");
			output = s_models.Get(name);

			return output;
		}

	public:
		static inline Pointer<Scene>& RemoveScene(const std::string& name)
		{
			static Pointer<Scene> output = nullptr;
			output = GetScene(name);
			s_scenes.Remove(name);

			return output;
		}

		static inline Pointer<Texture2D>& RemoveTexture2D(const std::string& name)
		{
			static Pointer<Texture2D> output = nullptr;
			output = GetTexture2D(name);
			s_textures.Remove(name);

			return output;
		}

		static inline Pointer<Material>& RemoveMaterial(const std::string& name)
		{
			static Pointer<Material> output = nullptr;
			output = GetMaterial(name);
			s_materials.Remove(name);

			return output;
		}

		static inline Pointer<Shader>& RemoveShader(const std::string& name)
		{
			static Pointer<Shader> output = nullptr;
			output = GetShader(name);
			s_shaders.Remove(name);

			return output;
		}

		static inline Pointer<StaticMeshModel>& RemoveStaticMeshModel(const std::string& name)
		{
			static Pointer<StaticMeshModel> output = nullptr;
			output = GetStaticMeshModel(name);
			s_models.Remove(name);

			return output;
		}

	public:
		static inline void AddScene(const std::string& name, Pointer<Scene> ptr)
		{
#ifdef DEBUG
			if (s_scenes.Contains(name))
				Logger::Warn("GlobalStorage::Add<Scene>: {0} already exists, Action = [Clear]", name);
#endif
			return s_scenes.Push(name, ptr);
		}

		static inline void AddTexture2D(const std::string& name, Pointer<Texture2D> ptr)
		{
#ifdef DEBUG
			if (s_scenes.Contains(name)) 
				Logger::Warn("GlobalStorage::Add<Texture2D>: {0} already exists, Action = [Clear]", name);
#endif
			return s_textures.Push(name, ptr);
		}

		static inline void AddMaterial(const std::string& name, Pointer<Material> ptr)
		{
#ifdef DEBUG
			if (s_scenes.Contains(name)) 
				Logger::Warn("GlobalStorage::Add<Material>: {0} already exists, Action = [Clear]", name);
#endif
			return s_materials.Push(name, ptr);
		}

		static inline void AddShader(const std::string& name, Pointer<Shader> ptr)
		{
#ifdef DEBUG
			if (s_scenes.Contains(name)) 
				Logger::Warn("GlobalStorage::Add<Shader>: {0} already exists, Action = [Clear]", name);
#endif
			return s_shaders.Push(name, ptr);
		}

		static inline void AddStaticMeshModel(const std::string& name, Pointer<StaticMeshModel> ptr)
		{
#ifdef DEBUG
			if (s_scenes.Contains(name)) 
				Logger::Warn("GlobalStorage::Add<StaticMeshModel>: {0} already exists, Action = [Clear]", name);
#endif
			return s_models.Push(name, ptr);
		}

	private:
		static Library<Pointer<Scene>> s_scenes;
		static Library<Pointer<Texture2D>> s_textures;
		static Library<Pointer<Material>> s_materials;
		static Library<Pointer<Shader>> s_shaders;
		static Library<Pointer<StaticMeshModel>> s_models;
    };
}