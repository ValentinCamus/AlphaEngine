#pragma once

#include <Alpha/Core/CoreMinimal.h>

#include <Alpha/Engine/Renderer/Cubemap.h>

#include <Alpha/Engine/Renderer/Shader.h>

#include <Alpha/Engine/Mesh/StaticMeshInstance.h>
#include <Alpha/Engine/GlobalStorage/GlobalStorage.h>

namespace Alpha
{
    class Skybox
    {
    public:
        enum EFaceOrientation
        {
            Right,
            Left,
            Top,
            Bottom,
            Back,
            Front
        };

    public:
        explicit inline Skybox(std::map<EFaceOrientation, std::string> faces)
        {
            ALPHA_ASSERT(faces.size() == 6, "Skybox invalid size: {0}", faces.size());

            std::vector<std::string> paths = {
                    faces[EFaceOrientation::Right],
                    faces[EFaceOrientation::Left],
                    faces[EFaceOrientation::Top],
                    faces[EFaceOrientation::Bottom],
                    faces[EFaceOrientation::Front],
                    faces[EFaceOrientation::Back],
            };

            m_cubemap = Cubemap::Create(paths);

            m_cube = NewPointer<StaticMeshInstance>("Skybox", GlobalStorage::GetStaticMeshModel("Cube"));
        }

        inline void Draw(const Pointer<Shader>& shader,
                         const Matrix4x4 * projection = nullptr,
                         const Matrix4x4 * view = nullptr)
        {
            ALPHA_ASSERT(view, "Skybox::Draw: Invalid view matrix");
            ALPHA_ASSERT(projection, "Skybox::Draw: Invalid projection matrix");

            m_cubemap->Bind(0);
            shader->SetUniform("u_skybox", 0);

            m_cube->Draw(shader, projection, view, nullptr);

            m_cubemap->Unbind();
        }

        inline const Pointer<Cubemap>& GetCubemap() const { return m_cubemap; }

    private:
        Pointer<Cubemap> m_cubemap;
        Pointer<StaticMeshInstance> m_cube;
    };
}
