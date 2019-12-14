#include <utility>

#pragma once

#include <Alpha/Core/CoreMinimal.h>

#include <Alpha/Engine/Animation/KeyFrame.h>

namespace Alpha
{
    class BoneAnimation
    {
    public:
        explicit BoneAnimation(std::vector<KeyFrame> keyFrames) : m_keyFrames(std::move(keyFrames)) {}

    public:
        inline const std::vector<KeyFrame>& GetKeyFrames() const { return m_keyFrames; }

    private:
        /// All the keyframes for the animation, ordered by time of appearance in the animation.
        std::vector<KeyFrame> m_keyFrames;
    };
}
