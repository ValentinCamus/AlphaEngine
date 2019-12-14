#pragma once

#include <Alpha/Core/CoreMinimal.h>

#include <Alpha/Engine/Renderer/Shader.h>
#include <Alpha/Engine/Material/Material.h>
#include <Alpha/Engine/Component/SceneComponent.h>
#include <Alpha/Engine/Mesh/SkeletalMeshModel.h>

#include <Alpha/Engine/Animation/Animation.h>
#include <Alpha/Engine/Animation/KeyFrame.h>

namespace Alpha
{
    class SkeletalMeshInstance : public SceneComponent
    {
    public:
        /// Constructor.
        explicit SkeletalMeshInstance(std::string name, const Pointer<SkeletalMeshModel>& model);

        /// Draw the entity.
        /// The model matrix will be set with the instance's transform.
        void Draw(const Pointer<Shader>& shader,
                  const Matrix4x4 * projection = nullptr,
                  const Matrix4x4 * view = nullptr) const;

        /// Draw the entity.
        void Draw(const Pointer<Shader>& shader,
                  const Matrix4x4 * projection,
                  const Matrix4x4 * view,
                  const Matrix4x4 * model) const;

        /// Instructs this entity to carry out a given animation.
        inline void PlayAnim(const Pointer<Animation>& animation)
        {
            m_animationTime = 0.0f;
            m_currentAnimation = animation;
        }

        inline void TickUpdate(float deltaTime) { UpdateAnimation(deltaTime); }

    public:
        /// The number of material.
        inline uint32 GetNbMaterial() const { return static_cast<uint32>(m_materials.size()); }

        /// @getter: The entity's material.
        const Pointer<Material>& GetMaterial(uint32 index);

        /// @setter: The entity's material.
        void SetMaterial(uint32 index, const Pointer<Material>& mat);

        /// @getter: The entity name.
        inline const std::string& GetName() const override { return m_name; }

        /// @getter: The model matrix.
        /// @warning: Perform a calculation.
        Matrix4x4 GetModelMatrix() const;

        inline Renderer::EDrawMode GetDrawMode() const { return m_drawMode; }
        inline void SetDrawMode(Renderer::EDrawMode drawMode) { m_drawMode = drawMode; }

        inline void BindMaterials() const { Renderer::GetDrawOptions()->bUseMaterial = true; }
        inline void UnbindMaterials() const { Renderer::GetDrawOptions()->bUseMaterial = false; }

    private:
        void SetModelMaterials() const;

        inline void UpdateAnimation(float deltaTime)
        {
            if (m_currentAnimation)
            {
                std::map<std::string, Matrix4x4> pose;

                IncreaseAnimationTime(deltaTime);
                pose = CalculateCurrentAnimationPose();
                ApplyPoseToBones(pose, m_model->GetSkeleton()->root, Matrix4x4(1.0f));
            }
        }

        inline void IncreaseAnimationTime(float deltaTime)
        {
            m_animationTime += deltaTime;

            if (m_animationTime > m_currentAnimation->GetDuration())
            {
                m_animationTime %= m_currentAnimation->GetDuration();
            }
        }

        inline std::map<std::string, Matrix4x4> CalculateCurrentAnimationPose()
        {
            std::pair<KeyFrame, KeyFrame> frames = GetPreviousAndNextFrames();
            float progression = CalculateProgression(frames.first, frames.second);
            return InterpolatePoses(frames.first, frames.second, progression);
        }

        inline void ApplyPoseToBones(const std::map<std::string, Matrix4x4>& currentPose,
                                     const Pointer<Bone>& bone,
                                     const Matrix4x4& parentTransform)
        {
            const Matrix4x4& currentLocalTransform = currentPose[bone->GetName()];
            Matrix4x4 currentTransform = parentTransform * currentLocalTransform;

            for (const Pointer<Bone>& child : bone->GetChildren())
            {
                ApplyPoseToBones(currentPose, child, currentTransform);
            }

            currentTransform = bone->GetInverseOriginalTransform() * currentTransform;
            bone->SetAnimationTransform(currentTransform);
        }

        inline std::pair<KeyFrame, KeyFrame> GetPreviousAndNextFrames()
        {
            const std::vector<KeyFrame>& frames = m_currentAnimation->GetKeyFrames();
            const KeyFrame& previous = frames.at(0);
            const KeyFrame& next = frames.at(0);

            for (const KeyFrame& frame : frames)
            {
                next = frame;
                if (next.GetTimeStamp() > m_animationTime) break;
                previous = frame;
            }

            return { previous, next };
        }

        inline float CalculateProgression(const KeyFrame& prev, const KeyFrame& next)
        {
            float totalTime = next.GetTimeStamp() - prev.GetTimeStamp();
            float currentTime = m_animationTime - prev.GetTimeStamp();
            return currentTime / totalTime;
        }

        inline std::map<std::string, Matrix4x4> InterpolatePoses(const KeyFrame& prev, const KeyFrame& next, float t)
        {
            std::map<std::string, Matrix4x4> pose;

            std::vector<std::string> boneNames = Map::ExtractKeys(prev.GetBoneKeyFrames());

            for (const std::string& boneName : boneNames)
            {
                const BoneTransform& prevTransform = prev.GetBoneKeyFrames()[boneName];
                const BoneTransform& nextTransform = next.GetBoneKeyFrames()[boneName];

                BoneTransform currentTransform = BoneTransform::Lerp(prevTransform, nextTransform, t);
                pose.insert({ boneName, currentTransform.GetLocalTransform() });
            }

            return pose;
        }

    private:
        /// The entity name.
        std::string m_name;

        /// The model of the entity.
        Pointer<SkeletalMeshModel> m_model;

        /// The list of material to apply to the entity when it will be drawn.
        std::vector<Pointer<Material>> m_materials;

        Renderer::EDrawMode m_drawMode = Renderer::EDrawMode::Triangles;

        float m_animationTime;
        Pointer<Animation> m_currentAnimation;
    };
}