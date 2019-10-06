#include "GlobalStorage.h"

namespace Alpha
{
	Library<Pointer<Scene>> GlobalStorage::s_scenes = Library<Pointer<Scene>>();
	Library<Pointer<Texture2D>> GlobalStorage::s_textures = Library<Pointer<Texture2D>>();
	Library<Pointer<Material>> GlobalStorage::s_materials = Library<Pointer<Material>>();
	Library<Pointer<Shader>> GlobalStorage::s_shaders = Library<Pointer<Shader>>();
	Library<Pointer<StaticMeshModel>> GlobalStorage::s_models = Library<Pointer<StaticMeshModel>>();
}
