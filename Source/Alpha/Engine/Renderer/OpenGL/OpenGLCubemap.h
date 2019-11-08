#pragma once

#include <Alpha/Core/CoreMinimal.h>

#include <Alpha/Engine/Renderer/Cubemap.h>
#include <Alpha/Engine/Renderer/OpenGL/OpenGLTools.h>
#include <Alpha/Engine/Renderer/OpenGL/OpenGLRenderer.h>

namespace Alpha
{
    class OpenGLCubemap : public Cubemap
    {
    public:
        explicit OpenGLCubemap(const std::vector<std::string>& paths) { Init(paths); }

        ~OpenGLCubemap() = default;

        inline void Bind(int32 slot) override { GL_CHECK(glBindTexture(GL_TEXTURE_CUBE_MAP, m_id + slot)); }
        inline void Unbind() override { GL_CHECK(glBindTexture(GL_TEXTURE_CUBE_MAP, 0)); }

        inline uint32 GetId() const override { return m_id; }
        inline int32 GetSlot() const override { return m_slot; }

    private:
        void Init(const std::vector<std::string>& paths);

    private:
        /// The id of this cubemap.
        /// @note: unique.
        uint32 m_id = 0;

        uint32 m_slot;
    };
}