#include "OpenGLShader.h"

#include <regex>
#include <fstream>

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
        GL_CHECK(glDeleteProgram(m_id));
    }

    GLenum OpenGLShader::ShaderTypeFromString(const std::string& type)
    {
        if (type == "vertex") return GL_VERTEX_SHADER;
        if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;

        ALPHA_ASSERT(false, "Unknown shader type!");
        return 0;
    }

    std::string OpenGLShader::ShaderTypeToString(GLenum type)
    {
        switch (type)
        {
            case GL_VERTEX_SHADER: return "vertex";
            case GL_FRAGMENT_SHADER: return "fragment";
            default: return "undefined";
        }
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
        else ALPHA_ASSERT(false, "OpenGLShader::ReadFile: Could not open file '{0}'", filepath);

        return result;
    }

    std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
    {
        // std::string defines = ReadFile(PROJECT_SOURCE_DIR + "Shaders/Defines.glsl");
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

                ALPHA_ASSERT(false, "Compilation Error ({0}): {1}", ShaderTypeToString(type), infoLog.data());
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
        ALPHA_ASSERT(location > -1, "Shader::GetUniformLocation, uniform \"{0}\" not found.", name);

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
}
