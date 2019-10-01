#pragma once

// FIXME: Fix assimp!
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include <Alpha/Core/CoreMinimal.h>

#include <Alpha/Engine/Renderer/StaticMesh.h>

namespace Alpha
{
    class MeshLoader
    {
    public:
        static bool Load(const std::string& filename);

        static inline const std::vector<Pointer<StaticMesh>>& Get() { return s_meshes; }
        static inline void Flush() { s_meshes.clear(); }

    private:
        /// Process an Assimp node.
        static void ProcessNode(aiNode * node, const aiScene * scene);

        /// Process an Assimp mesh, i.e load and create a StaticMesh pointer from the aiMesh.
        static Pointer<StaticMesh> ProcessMesh(aiMesh * mesh, const aiScene * scene);

    private:
        static std::vector<Pointer<StaticMesh>> s_meshes;
    };
}