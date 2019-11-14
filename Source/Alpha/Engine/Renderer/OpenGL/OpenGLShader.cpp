#include "OpenGLShader.h"

#include <regex>
#include <fstream>

#include <Alpha/Engine/Light/SpotLight.h>
#include <Alpha/Engine/Light/PointLight.h>
#include <Alpha/Engine/Light/DirectionalLight.h>

namespace Alpha
{
    OpenGLShader::OpenGLShader(const std::string& name, const std::map<int32, std::string>& sources)
        : m_name(name)
    {
        std::unordered_map<GLenum, std::string> glSources = {};
        for (const auto& it: sources)
        {
            auto shaderId = (GLenum) it.first;
            std::string shaderSource = ReadFile(it.second);
            glSources.insert({shaderId, shaderSource});
        }

        ALPHA_ASSERT(!glSources.empty(), "OpenGLShader: Invalid sources");

        Compile(glSources);
    }

    OpenGLShader::~OpenGLShader()
    {
        GL_CHECK(glDeleteProgram(m_id));
    }

    std::string OpenGLShader::ReadFile(const std::string &path, uint32 level)
    {
        ALPHA_ASSERT(level < 32, "OpenGLShader::IncludeFile: Include recursion went too deep");

        std::ifstream fileStream(path);

        if (!fileStream.is_open())
        {
            Logger::Error("OpenGLShader::IncludeFile: File not found: {0}", path);
            ForceQuit(EXIT_FAILURE);
        }

        std::string line;
        std::stringstream ss;

        std::regex reg("^[ ]*#[ ]*include[ ]+[\"<](.*)[\">].*");
        size_t position = path.find_last_of('/');
        std::string folder = path.substr(0, position + 1);

        while (std::getline(fileStream, line))
        {
            std::smatch match;
            if (std::regex_search(line, match, reg))
            {
                std::string includePath = folder + match[1].str();
                ss << ReadFile(includePath, 0);
            }
            else ss << line << "\n";
        }

        return ss.str();

    }

    void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
    {
        GLuint program;
        std::vector<GLenum> glShaderIDs;

        GL_CHECK(program = glCreateProgram());

        for (auto& kv : shaderSources)
        {
            GLenum type = kv.first;
            GLuint shader = 0;
            const std::string& source = kv.second;

            GL_CHECK(shader = glCreateShader(type));

            const GLchar* sourceCStr = source.c_str();
            GL_CHECK(glShaderSource(shader, 1, &sourceCStr, nullptr));

            GL_CHECK(glCompileShader(shader));

            GLint isCompiled = 0;
            GL_CHECK(glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled));
            if (isCompiled == GL_FALSE)
            {
                GLint maxLength = 0;
                GL_CHECK(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength));

                std::vector<GLchar> infoLog(static_cast<unsigned long>(maxLength));
                GL_CHECK(glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]));

                GL_CHECK(glDeleteShader(shader));

                Logger::Error("({0}): {1}", type, infoLog.data());
                ALPHA_ASSERT(false, "OpenGLShader::Compile: Compilation Error");
                break;
            }

            GL_CHECK(glAttachShader(program, shader));
            glShaderIDs.push_back(shader);
        }

        m_id = program;

        // Link our program
        GL_CHECK(glLinkProgram(program));

        // Note the different functions here: glGetProgram* instead of glGetShader*.
        GLint isLinked = 0;
        GL_CHECK(glGetProgramiv(program, GL_LINK_STATUS, &isLinked));

        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            GL_CHECK(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength));

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(static_cast<unsigned long>(maxLength));
            GL_CHECK(glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]));

            // We don't need the program anymore.
            GL_CHECK(glDeleteProgram(program));

            for (auto id : glShaderIDs) { GL_CHECK(glDeleteShader(id)); }

            Logger::Error("{0}", infoLog.data());
            ALPHA_ASSERT(false, "Shader link failure!");
            return;
        }

        for (auto id : glShaderIDs) { GL_CHECK(glDetachShader(program, id)); }
    }

    int32 OpenGLShader::GetUniformLocation(const std::string& name)
    {
        int32 location = -1; // Where: -1 is the OpenGL NULL uniform location.
        const char * cname = ToCharArray(name);

        GL_CHECK(location = glGetUniformLocation((GLuint)m_id, cname));
        ALPHA_ASSERT(location > -1, "OpenGLShader::GetUniformLocation, uniform \"{0}\" not found.", name);

        return location;
    }

    void OpenGLShader::SetUniform(const std::string &name, bool v)
    {
        int32 location = GetUniformLocation(name);
        GL_CHECK(glUniform1i(location, v));
    }

    void OpenGLShader::SetUniform(const std::string &name, int32 v)
    {
        int32 location = GetUniformLocation(name);
        GL_CHECK(glUniform1i(location, v));
    }

    void OpenGLShader::SetUniform(const std::string &name, float v)
    {
        int32 location = GetUniformLocation(name);
        GL_CHECK(glUniform1f(location, v));
    }

    void OpenGLShader::SetUniform(const std::string &name, const Vector2 &v)
    {
        int32 location = GetUniformLocation(name);
        GL_CHECK(glUniform2f(location, v.x, v.y));
    }

    void OpenGLShader::SetUniform(const std::string &name, const Vector3 &v)
    {
        int32 location = GetUniformLocation(name);
        GL_CHECK(glUniform3f(location, v.x, v.y, v.z));
    }

    void OpenGLShader::SetUniform(const std::string &name, const Vector4 &v)
    {
        int32 location = GetUniformLocation(name);
        GL_CHECK(glUniform4f(location, v.x, v.y, v.z, v.a));
    }

    void OpenGLShader::SetUniform(const std::string &name, const Matrix4 &m)
    {
        int32 location = GetUniformLocation(name);
        GL_CHECK(glUniformMatrix4fv(location, 1, GL_FALSE, &m[0][0]));
    }

    void OpenGLShader::SetUniform(const std::string &name, const Pointer<Light> &light)
    {
        SetUniform(name + ".type", light->GetType());
        SetUniform(name + ".color", light->GetColor());
        SetUniform(name + ".transform", light->GetViewProjectionMatrix());
        SetUniform(name + ".shadowMap", light->GetDepthBuffer()->GetTexture()->GetSlot());

        switch (light->GetType())
        {
            case Light::LightType::Directional:
            {
                Pointer<DirectionalLight> dirLight = Cast<DirectionalLight>(light);
                ALPHA_ASSERT(dirLight, "OpenGLShader::SetUniform: Invalid cast (DirectionalLight)");

                SetUniform(name + ".directional.direction", dirLight->GetForwardVector());

                break;
            }
            case Light::LightType::Point:
            {
                Pointer<PointLight> pointLight = Cast<PointLight>(light);
                ALPHA_ASSERT(pointLight, "OpenGLShader::SetUniform: Invalid cast (PointLight)");

                SetUniform(name + ".point.position", pointLight->GetWorldLocation());

                SetUniform(name + ".point.attenuation.constant", pointLight->GetAttenuation().constant);
                SetUniform(name + ".point.attenuation.linear", pointLight->GetAttenuation().linear);
                SetUniform(name + ".point.attenuation.quadratic", pointLight->GetAttenuation().quadractic);

                break;
            }
            case Light::LightType::Spot:
            {
                Pointer<SpotLight> spotLight = Cast<SpotLight>(light);
                ALPHA_ASSERT(spotLight, "OpenGLShader::SetUniform: Invalid cast (SpotLight)");

                SetUniform(name + ".spot.position", spotLight->GetWorldLocation());
                SetUniform(name + ".spot.direction", spotLight->GetForwardVector());
                SetUniform(name + ".spot.cutOff", spotLight->GetCutOff());

                SetUniform(name + ".spot.attenuation.constant", spotLight->GetAttenuation().constant);
                SetUniform(name + ".spot.attenuation.linear", spotLight->GetAttenuation().linear);
                SetUniform(name + ".spot.attenuation.quadratic", spotLight->GetAttenuation().quadractic);

                break;
            }
        }
    }
}
