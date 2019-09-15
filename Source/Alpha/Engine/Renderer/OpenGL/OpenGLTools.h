#pragma once

#include <Alpha/Core/CoreMinimal.h>

#include <Alpha/ThirdParty/glad/glad.h>

namespace Alpha
{
#define GL_CHECK(_function_) \
    Alpha::OpenGL::ClearErrorBuffer(); \
    _function_; \
    Alpha::OpenGL::DumpErrorBuffer(#_function_, __FILE__, __LINE__); \

    namespace OpenGL
    {
        /// Cast a GLError to a string
        inline std::string ErrorToString(GLenum error)
        {
            switch (error) {
                case GL_NO_ERROR:
                    return "No error";
                case GL_INVALID_ENUM:
                    return "Invalid enum : An unacceptable value is specified for an enumerated argument.";
                case GL_INVALID_VALUE:
                    return "Invalid value : A numeric argument is out of range.";
                case GL_INVALID_OPERATION:
                    return "Invalid operation : The specified operation is not allowed in the current state.";
                case GL_INVALID_FRAMEBUFFER_OPERATION:
                    return "Invalid framebuffer operation : The framebuffer object is not complete.";
                case GL_OUT_OF_MEMORY:
                    return "Out of memory : There is not enough memory left to execute the command.";
                default:
                    return "Unknown OpenGL error.";
            }
        }

        /// Clear the GLErrorBuffer.
        inline void ClearErrorBuffer() { while (glGetError() != GL_NO_ERROR) {} }

        /// Dump each error in the GLErrorBuffer.
        /// @return: True, if an error has occurred.
        inline bool DumpErrorBuffer(const char *func, const char *file, uint32 line)
        {
            bool isBufferEmpty = true;
            while (GLenum error = glGetError())
            {
                isBufferEmpty = false;
                std::string errmsg = ErrorToString(error);
                Logger::Error("OpenGL : {0}. \n\tFile : {1}, \n\tin {2}, \n\tat line {3}.", errmsg, file, func, line);
            }
            ALPHA_ASSERT(isBufferEmpty, "OpenGL error(s)");
            return isBufferEmpty;
        }
    }
}
