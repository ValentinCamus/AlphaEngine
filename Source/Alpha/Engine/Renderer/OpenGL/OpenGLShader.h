#pragma once

#include <Alpha/Engine/Renderer/Shader.h>

#include <Alpha/Engine/Renderer/OpenGL/OpenGLTools.h>
#include <Alpha/Engine/Renderer/OpenGL/OpenGLRenderer.h>

namespace Alpha
{
    class OpenGLShader : public Shader
    {
    public:
        explicit OpenGLShader(const std::string& name, const std::map<int32, std::string>& sources);

        ~OpenGLShader() override;

        inline void Bind() const override { GL_CHECK(glUseProgram(m_id)); }

        inline void Unbind() const override { GL_CHECK(glUseProgram(0)); }

        /// @getter: The name of the shader.
        const std::string& GetName() const override { return m_name; };

    public:
        /// Set a uniform of the type : bool.
        void SetUniform(const std::string &name, bool v) override;

        /// Set a uniform of the type : int.
        void SetUniform(const std::string &name, int32 v) override;

        /// Set a uniform of the type : float.
        void SetUniform(const std::string &name, float v) override;

        /// Set a uniform of the type : Vector2.
        void SetUniform(const std::string &name, const Vector2 &v) override;

        /// Set a uniform of the type : Vector3.
        void SetUniform(const std::string &name, const Vector3 &v) override;

        /// Set a uniform of the type : Vector4.
        void SetUniform(const std::string &name, const Vector4 &v) override;

        /// Set a uniform of the type : Matrix4x4.
        void SetUniform(const std::string &name, const Matrix4 &m) override;

        /// Set a uniform of the type : Light.
        void SetUniform(const std::string &name, const Pointer<Light>& light) override;
    private:
        static std::string ReadFile(const std::string& filepath, uint32 level = 0);

        int32 GetUniformLocation(const std::string& name);

        void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);

    private:
        uint32 m_id = 0;
        std::string m_name;
    };
}