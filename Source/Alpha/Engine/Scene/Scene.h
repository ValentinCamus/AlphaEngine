#pragma once

#include <Alpha/Core/CoreMinimal.h>


#include <Alpha/Engine/Light/Light.h>
#include <Alpha/Engine/Camera/Camera.h>
#include <Alpha/Engine/Renderer/Framebuffer.h>
#include <Alpha/Engine/Component/SceneComponent.h>

namespace Alpha
{
    class Scene
    {
    public:
        inline void Bind() const { m_framebuffer->Bind(); }
        inline void Unbind() const { m_framebuffer->Unbind(); }

        inline const Pointer<Camera>& GetCamera() const { return m_camera; }
        inline void SetCamera(const Pointer<Camera>& camera) { m_camera = camera; }

        inline const Pointer<Framebuffer>& GetFramebuffer() const { return m_framebuffer; }
        inline void SetFramebuffer(const Pointer<Framebuffer>& fb) { m_framebuffer = fb; }

        inline const std::vector<Pointer<Light>>& GetLights() const { return m_lights; }
        inline const Pointer<Light>& GetLightAt(uint32 index) const { return m_lights[index]; }
        inline void SetLightAt(uint32 index, const Pointer<Light>& light) { m_lights[index] = light; }
        inline uint32 GetNbLights() const { return m_lights.size(); }

        inline const std::vector<Pointer<SceneComponent>>& GetComponents() const { return m_components; }
        inline const Pointer<SceneComponent>& GetComponentAt(uint32 index) const { return m_components[index]; }
        inline void SetComponentAt(uint32 index, const Pointer<SceneComponent>& sc) { m_components[index] = sc; }
        inline uint32 GetNbComponents() const { return m_components.size(); }

    private:
        /// The main camera of the scene.
        Pointer<Camera> m_camera;

        /// The framebuffer of the scene.
        Pointer<Framebuffer> m_framebuffer;

        /// All the lights that compose the scene.
        std::vector<Pointer<Light>> m_lights;

        /// All the components of the scene.
        std::vector<Pointer<SceneComponent>> m_components;
    };
}