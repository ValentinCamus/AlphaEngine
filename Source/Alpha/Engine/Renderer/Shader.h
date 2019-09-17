#pragma once

#include <Alpha/Core/CoreMinimal.h>

#include <Alpha/Engine/Light/Light.h>

namespace Alpha
{
    class Shader
    {
    public:
        static const int32 GLSL_VERTEX_SHADER;
        static const int32 GLSL_FRAGMENT_SHADER;
        static const int32 GLSL_GEOMETRY_SHADER;

    public:
        static Pointer<Shader> Create(const std::string& name, const std::map<int32, std::string>& sources);

    public:
        virtual ~Shader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        /// @getter: The name of the shader.
        virtual const std::string& GetName() const = 0;

        /// Set a uniform of the type : bool.
        virtual void SetUniform(const std::string &name, bool v) = 0;

        /// Set a uniform of the type : int.
        virtual void SetUniform(const std::string &name, int32 v) = 0;

        /// Set a uniform of the type : float.
        virtual void SetUniform(const std::string &name, float v) = 0;

        /// Set a uniform of the type : Vector2.
        virtual void SetUniform(const std::string &name, const Vector2 &v) = 0;

        /// Set a uniform of the type : Vector3.
        virtual void SetUniform(const std::string &name, const Vector3 &v) = 0;

        /// Set a uniform of the type : Vector4.
        virtual void SetUniform(const std::string &name, const Vector4 &v) = 0;

        /// Set a uniform of the type : Matrix4x4.
        virtual void SetUniform(const std::string &name, const Matrix4 &m) = 0;

        /// Set a uniform of the type : Light.
        virtual void SetUniform(const std::string &name, const Pointer<Light>& light) = 0;
    };
}