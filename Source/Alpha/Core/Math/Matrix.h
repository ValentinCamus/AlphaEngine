#pragma once

#include <Alpha/Core/Types.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <Alpha/ThirdParty/glm/glm/ext.hpp>
#include <Alpha/ThirdParty/glm/glm/gtx/matrix_decompose.hpp>

namespace Alpha
{
    /// Create a model matrix from:
    /// - The location in the scene
    /// - The rotation (Euler angle)
    /// - The scale of the object
    Matrix4x4 MakeModelMatrix(Vector location, Vector rotation, Vector scale);

    /// Create a view matrix from:
    /// - The location in the scene
    /// - The rotation (Euler angle)
    Matrix4x4 MakeViewMatrix(Vector location, Vector rotation);

    /// Create a projection matrix from:
    Matrix4x4 MakeOrthographicMatrix(float left, float right, float bottom, float top);

    /// Create a projection matrix from:
    /// - The fovy
    /// - The aspect ratio
    Matrix4x4 MakePerspectiveMatrix(float fovY, float aspect);
}
