#include "Math.h"

namespace Alpha
{
    template<class T>
    T MapRange(T value, float rangeMin, float rangeMax, float newRangeMin, float newRangeMax)
    {
        float numerator = (value - rangeMin)  * (newRangeMax - newRangeMin);
        float denominator = rangeMax - rangeMin + newRangeMin;
        return numerator / denominator;
    }

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

    Matrix4x4 MakeProjectionMatrix(float fovy, float aspect, float zNear, float zFar)
    {
        return glm::perspective(fovy, aspect, zNear, zFar);
    }

    Vector3 CalculateRightVector(const Transform &transform)
    {
        const Vector& location = transform.location;
        const Vector& rotation = transform.rotation;
        const Vector& scale = transform.scale;
        Matrix4x4 matrix = MakeModelMatrix(location, rotation, scale);
        Matrix4x4 inverted = glm::inverse(matrix);
        Vector3 right = Vector3(inverted[0]);
        return glm::normalize(right);
    }

    Vector3 CalculateUpVector(const Transform &transform)
    {
        const Vector& location = transform.location;
        const Vector& rotation = transform.rotation;
        const Vector& scale = transform.scale;
        Matrix4x4 matrix = MakeModelMatrix(location, rotation, scale);
        Matrix4x4 inverted = glm::inverse(matrix);
        Vector3 right = -Vector3(inverted[1]);
        return glm::normalize(right);
    }

    Vector3 CalculateForwardVector(const Transform &transform)
    {
        const Vector& location = transform.location;
        const Vector& rotation = transform.rotation;
        const Vector& scale = transform.scale;
        Matrix4x4 matrix = MakeModelMatrix(location, rotation, scale);
        Matrix4x4 inverted = glm::inverse(matrix);
        Vector3 forward = -Vector3(inverted[2]);
        return glm::normalize(forward);
    }
}
