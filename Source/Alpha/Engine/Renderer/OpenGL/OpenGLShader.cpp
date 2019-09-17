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
        std::unordered_map<GLenum, std::string> glSources;
        for (auto& it: sources)
        {
            GLenum shaderId = it.first;
            std::string shaderSource = ReadFile(it.second);
            glSources.insert({shaderId, shaderSource});
        }

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
        OpenGL::ClearErrorBuffer();
        GLuint program = glCreateProgram();
        OpenGL::DumpErrorBuffer("OpenGLShader::Compile", __FILE__, __LINE__);

        std::vector<GLenum> glShaderIDs;

        int glShaderIDIndex = 0;
        for (auto& kv : shaderSources)
        {
            GLenum type = kv.first;
            const std::string& source = kv.second;

            OpenGL::ClearErrorBuffer();
            GLuint shader = glCreateShader(type);
            OpenGL::DumpErrorBuffer("OpenGLShader::Compile", __FILE__, __LINE__);

            const GLchar* sourceCStr = source.c_str();
            GL_CHECK(glShaderSource(shader, 1, &sourceCStr, nullptr));

            GL_CHECK(glCompileShader(shader));

            GLint isCompiled = 0;
            GL_CHECK(glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled));
            if (isCompiled == GL_FALSE)
            {
                GLint maxLength = 0;
                GL_CHECK(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength));

                std::vector<GLchar> infoLog(maxLength);
                GL_CHECK(glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]));

                GL_CHECK(glDeleteShader(shader));

                ALPHA_ASSERT(false, "Compilation Error ({0}):\n{1}", type, infoLog.data());
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
        GL_CHECK(glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked));
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            GL_CHECK(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength));

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            GL_CHECK(glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]));

            // We don't need the program anymore.
            GL_CHECK(glDeleteProgram(program));

            for (auto id : glShaderIDs) { GL_CHECK(glDeleteShader(id)); }

            ALPHA_ASSERT("{0}", infoLog.data());
            ALPHA_ASSERT(false, "Shader link failure!");
            return;
        }

        for (auto id : glShaderIDs) { GL_CHECK(glDetachShader(program, id)); }
    }

    int32 OpenGLShader::GetUniformLocation(const std::string& name)
    {
        const char * cname = ToCharArray(name);

        OpenGL::ClearErrorBuffer();
        int32 location = glGetUniformLocation((GLuint)m_id, cname);
        OpenGL::DumpErrorBuffer("OpenGLShader::Compile: glGetUniformLocation", __FILE__, __LINE__);
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
        Pointer<DirectionalLight> dirLight = nullptr;
        Pointer<PointLight> pointLight = nullptr;
        Pointer<SpotLight> spotLight = nullptr;

        SetUniform(name + ".type", light->GetType());
        SetUniform(name + ".color", light->GetColor());

        switch (light->GetType())
        {
            case Light::LightType::Directional:
                dirLight = Cast<DirectionalLight>(light);

                if (dirLight == nullptr)
                {
                    Logger::Error("OpenGLShader::SetUniform (Light --> Dir) : Invalid cast");
                    ForceQuit(EXIT_FAILURE);
                }

                SetUniform(name + ".directional.direction", dirLight->GetDirection());

                break;

            case Light::LightType::Point:
                pointLight = Cast<PointLight>(light);

                if (pointLight == nullptr)
                {
                    Logger::Error("OpenGLShader::SetUniform (Light --> Point) : Invalid cast");
                    ForceQuit(EXIT_FAILURE);
                }

                SetUniform(name + ".point.position", pointLight->GetWorldLocation());
                SetUniform(name + ".point.attenuation.constant", pointLight->GetAttenuation().constant);
                SetUniform(name + ".point.attenuation.linear", pointLight->GetAttenuation().linear );
                SetUniform(name + ".point.attenuation.quadratic", pointLight->GetAttenuation().quadratic );

                break;

            case Light::LightType::Spot:
                spotLight = Cast<SpotLight>(light);

                if (spotLight == nullptr)
                {
                    Logger::Error("OpenGLShader::SetUniform (Light --> Spot) : Invalid cast");
                    ForceQuit(EXIT_FAILURE);
                }

                SetUniform(name + ".spot.position", spotLight->GetWorldLocation());
                SetUniform(name + ".spot.direction", spotLight->GetDirection());
                SetUniform(name + ".spot.innerAngle", spotLight->GetInnerAngle());
                SetUniform(name + ".spot.outerAngle", spotLight->GetOuterAngle());
                SetUniform(name + ".spot.attenuation.constant", spotLight->GetAttenuation().constant);
                SetUniform(name + ".spot.attenuation.linear", spotLight->GetAttenuation().linear );
                SetUniform(name + ".spot.attenuation.quadratic", spotLight->GetAttenuation().quadratic );

                break;
        }
    }
}
