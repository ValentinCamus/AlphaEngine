#include "OpenGLShader.h"

#include <fstream>

#define UNDEFINED_UNIFORM_LOCATION -1

namespace Alpha
{
    OpenGLShader::OpenGLShader(const std::string &filepath)
            : m_filepath(filepath)
    {
        std::string source = ReadFile(filepath);
        auto shaderSources = PreProcess(source);
        Compile(shaderSources);

        // Extract name from filepath.
        auto lastSlash = filepath.find_last_of("/\\");
        lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
        auto lastDot = filepath.rfind('.');
        auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
        m_name = filepath.substr(lastSlash, count);
    }

    OpenGLShader::~OpenGLShader()
    {
        glDeleteProgram(m_id);
    }

    GLenum OpenGLShader::ShaderTypeFromString(const std::string& type)
    {
        if (type == "vertex") return GL_VERTEX_SHADER;
        if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;

        ALPHA_ASSERT(false, "Unknown shader type!");
        return 0;
    }

    std::string OpenGLShader::ReadFile(const std::string& filepath)
    {
        std::string result;
        std::ifstream in(filepath, std::ios::in | std::ios::binary);
        if (in)
        {
            in.seekg(0, std::ios::end);
            result.resize((uint32) in.tellg());
            in.seekg(0, std::ios::beg);
            in.read(&result[0], result.size());
            in.close();
        }
        else ALPHA_ASSERT("Could not open file '{0}'", filepath);

        return result;
    }

    std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
    {
        std::unordered_map<GLenum, std::string> shaderSources;

        const char* typeToken = "#type";
        size_t typeTokenLength = strlen(typeToken);
        size_t pos = source.find(typeToken, 0);
        while (pos != std::string::npos)
        {
            size_t eol = source.find_first_of("\r\n", pos);
            ALPHA_ASSERT(eol != std::string::npos, "Syntax error");
            size_t begin = pos + typeTokenLength + 1;
            std::string type = source.substr(begin, eol - begin);
            ALPHA_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

            size_t nextLinePos = source.find_first_not_of("\r\n", eol);
            pos = source.find(typeToken, nextLinePos);

            uint32 sourceCount = pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos);
            shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, sourceCount);
        }

        return shaderSources;
    }

    void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
    {
        GLuint program = glCreateProgram();
        std::vector<GLenum> glShaderIDs(shaderSources.size());

        int glShaderIDIndex = 0;
        for (auto& kv : shaderSources)
        {
            GLenum type = kv.first;
            const std::string& source = kv.second;

            GLuint shader = glCreateShader(type);

            const GLchar* sourceCStr = source.c_str();
            glShaderSource(shader, 1, &sourceCStr, nullptr);

            glCompileShader(shader);

            GLint isCompiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
            if (isCompiled == GL_FALSE)
            {
                GLint maxLength = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

                std::vector<GLchar> infoLog(maxLength);
                glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

                glDeleteShader(shader);

                ALPHA_ASSERT("{0}", infoLog.data());
                ALPHA_ASSERT(false, "Shader compilation failure!");
                break;
            }

            glAttachShader(program, shader);
            glShaderIDs[glShaderIDIndex++] = shader;
        }

        m_id = program;

        // Link our program
        glLinkProgram(program);

        // Note the different functions here: glGetProgram* instead of glGetShader*.
        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

            // We don't need the program anymore.
            glDeleteProgram(program);

            for (auto id : glShaderIDs) glDeleteShader(id);

            ALPHA_ASSERT("{0}", infoLog.data());
            ALPHA_ASSERT(false, "Shader link failure!");
            return;
        }

        for (auto id : glShaderIDs) glDetachShader(program, id);
    }

    int32 OpenGLShader::GetUniformLocation(const std::string& name)
    {
        int32 location;
        const char * cname = name.c_str();

        location = glGetUniformLocation((GLuint)m_id, cname);


        if (location == UNDEFINED_UNIFORM_LOCATION)
        {
            Logger::Error("Shader::GetUniformLocation, uniform : {0} not found.", name);
            ALPHA_ASSERT(false, "Undefined uniform");
        }

        return location;
    }

    void OpenGLShader::SetUniform(const std::string &name, bool v)
    {
        int32 location = GetUniformLocation(name);
        glUniform1i(location, v);
    }

    void OpenGLShader::SetUniform(const std::string &name, int32 v)
    {
        int32 location = GetUniformLocation(name);
        glUniform1i(location, v);
    }

    void OpenGLShader::SetUniform(const std::string &name, float v)
    {
        int32 location = GetUniformLocation(name);
        glUniform1f(location, v);
    }

    void OpenGLShader::SetUniform(const std::string &name, const Vector2 &v)
    {
        int32 location = GetUniformLocation(name);
        glUniform2f(location, v.x, v.y);
    }

    void OpenGLShader::SetUniform(const std::string &name, const Vector3 &v)
    {
        int32 location = GetUniformLocation(name);
        glUniform3f(location, v.x, v.y, v.z);
    }

    void OpenGLShader::SetUniform(const std::string &name, const Vector4 &v)
    {
        int32 location = GetUniformLocation(name);
        glUniform4f(location, v.x, v.y, v.z, v.a);
    }

    void OpenGLShader::SetUniform(const std::string &name, const Matrix4 &m)
    {
        int32 location = GetUniformLocation(name);
        glUniformMatrix4fv(location, 1, GL_FALSE, &m[0][0]);
    }

    void OpenGLShader::SetUniform(const std::string &name, const TransformMatrix &m)
    {
        int32 location = GetUniformLocation(name);

        const Matrix4x4  mvp = m.projection * m.view * m.model;

        SetUniform(name + ".model", m.model);
        SetUniform(name + ".view",  m.view);
        SetUniform(name + ".proj", m.projection);
        SetUniform(name + ".mvp", mvp);
    }
}
