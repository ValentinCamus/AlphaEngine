#pragma once

#include <Alpha/Core/CoreMinimal.h>
#include <Alpha/Engine/Light/Light.h>

namespace Alpha
{
    class DirectionalLight : public Light
    {
    public:
        /// Constructor.
        explicit DirectionalLight(const Vector3& direction = Vector3(0.0f, 0.0f, 0.0f),
                                  const Color4& color = Color4(1.0f))
            : Light(Directional, Vector(0.0f, 0.0f, 0.0f), direction, color)
        {

        }

        /// Destructor.
        ~DirectionalLight() override = default;
    };
}