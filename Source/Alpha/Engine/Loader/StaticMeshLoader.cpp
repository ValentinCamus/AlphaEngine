#include "StaticMeshLoader.h"

namespace Alpha
{

    std::vector<Pointer<StaticMesh>> StaticMeshLoader::s_meshes = std::vector<Pointer<StaticMesh>>();

    bool StaticMeshLoader::Load(const std::string& filename)
    {
        Assimp::Importer importer;
        const aiScene * scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs);

        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            Logger::Error("Assimp: {0}", importer.GetErrorString());
            return false;
        }
        else
        {
            s_meshes.clear();
            ProcessNode(scene->mRootNode, scene);
            return true;
        }
    }

    void StaticMeshLoader::ProcessNode(aiNode * node, const aiScene * scene)
    {
        // Process all the node's meshes (if any)
        for (uint32 i = 0; i < node->mNumMeshes; ++i)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            s_meshes.push_back(ProcessMesh(mesh, scene));
        }

        // Then do the same for each of its children
        for (uint32 i = 0; i < node->mNumChildren; i++)
        {
            ProcessNode(node->mChildren[i], scene);
        }
    }

    Pointer<StaticMesh> StaticMeshLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene)
    {
        ALPHA_UNUSED(scene); // Here, scene can be used to load the model's materials.

        std::vector<uint32> indices;
        std::vector<StaticMesh::Vertex> vertices;

        for (uint32 i = 0; i < mesh->mNumVertices; i++)
        {
            // Process vertex positions, normals and texture coordinates
            Vector position;
            position.x = mesh->mVertices[i].x;
            position.y = mesh->mVertices[i].y;
            position.z = mesh->mVertices[i].z;

            Vector normal;
            normal.x = mesh->mNormals[i].x;
            normal.y = mesh->mNormals[i].y;
            normal.z = mesh->mNormals[i].z;

            Vector2 texCoords;
            if (mesh->mTextureCoords[0]) // Does the mesh contains texture coordinates?
            {
                texCoords.x = mesh->mTextureCoords[0][i].x;
                texCoords.y = 1 - mesh->mTextureCoords[0][i].y;
            }
            else texCoords = glm::vec2(0.0f, 0.0f);

            vertices.emplace_back(position, normal, texCoords);
        }

        // Process indices
        for (uint32 i = 0; i < mesh->mNumFaces; ++i)
        {
            aiFace face = mesh->mFaces[i];
            for(uint32 j = 0; j < face.mNumIndices; j++)
            {
                indices.push_back(face.mIndices[j]);
            }
        }

        return StaticMesh::Create(vertices, indices);
    }

}