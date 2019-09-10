#pragma once

#include <cstdint>

#include <map>
#include <list>
#include <stack>
#include <vector>
#include <string>

#include <Alpha/ThirdParty/glm/glm/glm.hpp>

#define SMALL_NUMBER (1.e-8f)
#define BIG_NUMBER (3.4e+38f)

#define INV_PI (0.31830988618f)
#define HALF_PI (1.57079632679f)

namespace Alpha
{
	// ============================================= 
	// Native Types                             
	// ============================================= 

	using int8 = std::int8_t;
	using int16 = std::int16_t;
	using int32 = std::int32_t;
	using int64 = std::int64_t;

	using uint8 = std::uint8_t;
	using uint16 = std::uint16_t;
	using uint32 = std::uint32_t;
	using uint64 = std::uint64_t;

	using uchar = unsigned char;

	// ============================================= 
	// Vectores                             
	// ============================================= 

	using Point2 = glm::vec2;
	using Vector2 = glm::vec2;

	using Point = glm::vec3;
	using Vector = glm::vec3;
	using Point3 = glm::vec3;
	using Vector3 = glm::vec3;

	using Point4 = glm::vec4;
	using Vector4 = glm::vec4;

	using IntPoint2 = glm::ivec2;
	using IntVector2 = glm::ivec2;

	using IntPoint = glm::ivec3;
	using IntVector = glm::vec3;
	using IntPoint3 = glm::ivec3;
	using IntVector3 = glm::vec3;

	using IntPoint4 = glm::ivec4;
	using IntVector4 = glm::ivec4;

	using Color4 = Vector4;

	using Quat = glm::quat;

	// ============================================= 
	// Matrices                             
	// ============================================= 

	using Matrix2x2 = glm::mat2x2;
	using Matrix2x3 = glm::mat2x3;
	using Matrix2x4 = glm::mat2x4;
	using Matrix2 = glm::mat2x2;

	using Matrix3x2 = glm::mat3x2;
	using Matrix3x3 = glm::mat3x3;
	using Matrix3x4 = glm::mat3x4;
	using Matrix3 = glm::mat3x3;

	using Matrix4x2 = glm::mat4x2;
	using Matrix4x3 = glm::mat4x3;
	using Matrix4x4 = glm::mat4x4;
	using Matrix4 = glm::mat4x4;

	// ============================================= 
	// Custom Types                             
	// ============================================= 

	struct BoundingBox
	{
		Vector min;
		Vector max;
	};

	struct MatrixTransform
	{
		Matrix4x4 model;
		Matrix4x4 view;
		Matrix4x4 projection;
	};

	struct Transform
	{
		Vector location;
		Vector rotation; // Euler angles
		Vector scale;
	};

	struct Vertex
	{
		Vector position;
		Vector normal;
		Vector2 texCoords;
	};

	template<typename T> using Pointer = std::shared_ptr<T>;
}