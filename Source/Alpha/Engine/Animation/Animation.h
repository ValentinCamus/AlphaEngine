#include <utility>

#pragma once

#include <Alpha/Core/CoreMinimal.h>

#include <Alpha/Engine/Animation/BoneAnimation.h>

namespace Alpha
{
    class Animation
    {
    public:
        inline Animation(float duration, std::vector<BoneAnimation> boneAnimations)
            : m_duration(duration)
            , m_boneAnimations(std::move(boneAnimations))
        {

        }

        inline float GetDuration() const { return m_duration; }

        inline const std::vector<BoneAnimation>& GetBoneAnimations() const { return m_boneAnimations; }

    private:
        /// The total length of the animation in seconds.
        float m_duration;

        /// All the bone animation for the animation, ordered by time of appearance in the animation.
        std::vector<BoneAnimation> m_boneAnimations;
    };
}
