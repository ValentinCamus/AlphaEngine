#include "Matrix.h"

#include <Alpha/Engine/Renderer/Renderer.h>

namespace Alpha
{
    Matrix4x4 MakeModelMatrix(Vector location, Vector rotation, Vector scale)
    {
        Matrix4x4 matrix(1.0f);

        matrix = glm::translate(matrix, location);

        matrix = glm::rotate(matrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        matrix = glm::rotate(matrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        matrix = glm::rotate(matrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

        matrix = glm::scale(matrix, scale);

        return matrix;
    }

    Matrix4x4 MakeViewMatrix(Vector location, Vector rotation)
    {
        Matrix4x4 viewMatrix(1.0f);

        // Rotate the view matrix.
        viewMatrix = glm::rotate(viewMatrix, glm::radians(rotation.x), Vector(1, 0, 0));
        viewMatrix = glm::rotate(viewMatrix, glm::radians(rotation.y), Vector(0, 1, 0));
        viewMatrix = glm::rotate(viewMatrix, glm::radians(rotation.z), Vector(0, 0, 1));

        // Invert the view so when we move left, the view goes to the right.
        viewMatrix = glm::translate(viewMatrix, -location);

        return viewMatrix;
    }

    Matrix4x4 MakePerspectiveMatrix(float fovY, float aspect)
    {
        return glm::perspective(fovY, aspect, Renderer::GetZNear(), Renderer::GetZFar());
    }

    Matrix4x4 MakeOrthographicMatrix(float left, float right, float bottom, float top)
    {
        return glm::ortho(left, right, bottom, top, Renderer::GetZNear(), Renderer::GetZFar());
    }
}