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
