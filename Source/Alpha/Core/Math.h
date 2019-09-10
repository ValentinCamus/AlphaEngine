#pragma once

#include <cmath>

#define GLM_ENABLE_EXPERIMENTAL
#include <Alpha/ThirdParty/glm/glm/ext.hpp>
#include <Alpha/ThirdParty/glm/glm/gtx/matrix_decompose.hpp>

#include <Alpha/Core/Types.h>

namespace Alpha
{
	/// @return: A non-zero value (true) if the sign of x is negative and zero (false) otherwise.
	template<class T>
	inline int32 Sign(const T& value) { return std::signbit(value) ? -1 : 1; }

	/// Calculate the power of two of @value.
	template<class T>
	inline T Pow2(T value) { return value * value; }

	/// Calculate the power of three of @value.
	template<class T>
	inline T Pow3(T value) { return value * value * value; }

	/// Clamp @value between @min and @max.
	template<class T>
	inline T Clamp(T value, T min, T max) { return std::max(std::min(value, max), min); }

	/// Check if @value is between @min and @max.
	template<class T>
	inline bool InRange(T value, T min, T max) { return (value >= min && value <= max); }

	/// Calculate the factorial of @value.
	template<class T>
	inline T Fact(T value) { return (value > 0) ? value * Fact(value - 1) : 1; }

	/// Returns Value mapped from one range into another where the Value is clamped to the Input Range.
	/// (e.g. 0.5 normalized from the range 0->1 to 0->50 would result in 25)
	template<class T>
	inline T MapRange(T value, float rangeMin, float rangeMax, float newRangeMin, float newRangeMax)
	{
		float numerator = (value - rangeMin)  * (newRangeMax - newRangeMin);
		float denominator = rangeMax - rangeMin + newRangeMin;
		return numerator / denominator;
	}

	/// Create a model matrix from:
	/// - The location in the scene
	/// - The rotation (Euler angle)
	/// - The scale of the object
	inline Matrix4x4 MakeModelMatrix(Vector location, Vector rotation, Vector scale)
	{
		Matrix4x4 matrix(1.0f);

		matrix = glm::translate(matrix, location);

		matrix = glm::rotate(matrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		matrix = glm::rotate(matrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		matrix = glm::rotate(matrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

		matrix = glm::scale(matrix, scale);

		return matrix;
	}

	/// Create a view matrix from:
	/// - The location in the scene
	/// - The rotation (Euler angle)
	inline Matrix4x4 MakeViewMatrix(Vector location, Vector rotation)
	{
		Matrix4x4 viewMatrix(1.0f);

		// Rotate the view matrix.
		viewMatrix = glm::rotate(viewMatrix, glm::radians(rotation.x), Vector(1, 0, 0));
		viewMatrix = glm::rotate(viewMatrix, glm::radians(rotation.y), Vector(0, 1, 0));

		// Invert the view so when we move left, the view goes to the right.
		viewMatrix = glm::translate(viewMatrix, -location);

		return viewMatrix;
	}
}