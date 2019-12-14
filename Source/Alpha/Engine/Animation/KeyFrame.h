#include <utility>

#pragma once

#include <Alpha/Core/CoreMinimal.h>

#include <Alpha/Engine/Animation/BoneTransform.h>

namespace Alpha
{
    class KeyFrame
    {
    public:
        inline KeyFrame(float timeStamp, std::map<std::string, BoneTransform> boneKeyFrames)
            : m_timeStamp(timeStamp)
            , m_pose(std::move(boneKeyFrames))
        {

        }

        inline float GetTimeStamp() const { return m_timeStamp; }

        inline const std::map<std::string, BoneTransform>& GetBoneKeyFrames() const { return m_pose; }

    private:
        /// The time in seconds of the keyframe in the animation.
        float m_timeStamp;

        /// The desired bone-space transforms of all the bones at this keyframe,
        /// of the animation, indexed by the name of the bone that they correspond to.
        /// This basically represents the "pose" at this keyframe.
        std::map<std::string, BoneTransform> m_pose;
    };
}

