#pragma once

#include <Alpha/Core/CoreMinimal.h>

#include <Alpha/Engine/Renderer/Shader.h>
#include <Alpha/Engine/Renderer/Texture.h>

namespace Alpha
{
    class Material
    {
    public:
        /// The type of the texture.
        enum class ETextureType
        {
            TX_Diffuse,
            TX_Specular,
            TX_Normal,
            TX_Roughness,
            TX_Transparency
        };

    public:
        /// Constructor.
        /// @name: The uniform name.
        explicit Material(const std::string &name = "material") : m_name(name)
        {
            Logger::Info("Creating Material: {0}", name);
        }

        /// Destructor.
        virtual ~Material() { m_textures.clear(); }

        /// Bind this material.
        virtual void Bind(const Pointer<Shader>& shader, const std::string& uniformName);

        /// Unbind this material.
        virtual void Unbind();

    public:
        /// Add a new texture.
        inline void AddTexture(const ETextureType& type, const Pointer<Texture2D>& texture)
        {
            m_textures.insert({type, texture});
        }

        /// Remove a texture.
        /// @return: False is texture not found.
        inline bool RemoveTexture(const ETextureType& type)
        {
            auto it = m_textures.find(type);
            bool isFound = (it != m_textures.end());

            if (isFound) m_textures.erase(type);

            return isFound;
        }

        /// Clear all the textures.
        inline void ClearTextures() { m_textures.clear(); }

        /// Get the texture that match the type in argument.
        inline Pointer<Texture2D> GetTexture(const ETextureType& type)
        {
            auto it = m_textures.find(type);
            return (it != m_textures.end()) ? it->second : nullptr;
        }

    public:
        /// @getter: The Kd color of the material.
        inline const Vector4& GetKd() const { return m_kd; }

        /// @setter: The Kd color of the material.
        inline const Vector4& SetKd(const Vector4& kd) { return m_kd = kd; }

        /// @getter: The Ks color of the material.
        inline const Vector4& GetKs() const { return m_ks; }

        /// @setter: The Ks color of the material.
        inline const Vector4& SetKs(const Vector4& ks) { return m_ks = ks; }

        /// @getter: The roughness of the material.
        inline const Vector3& GetNormal() const { return m_normal; }

        /// @setter: The roughness of the material.
        inline const Vector3& SetNormal(const Vector3& normal) { return m_normal = normal; }

        /// @getter: The roughness of the material.
        inline float GetRoughness() const { return m_roughness; }

        /// @setter: The roughness of the material.
        inline float SetRoughness(float roughness) { return m_roughness = roughness; }

        /// @getter: The transparency of the material.
        inline float GetTransparency() const { return m_transparency; }

        /// @setter: The transparency of the material.
        inline float SetTransparency(float transparency) { return m_transparency = transparency; }

    private:
        /// The name of the uniform.
        std::string m_name;

        /// The Kd (diffuse) color.
        /// @note: m_kd color has a lower priority than TX_Diffuse.
        Vector4 m_kd = Vector4(1);

        /// The Ks (specular) color.
        /// @note: m_ks color has a lower priority than TX_Specular.
        Vector4 m_ks = Vector4(1);

        /// The normal color.
        /// @note: m_normal color has a lower priority than TX_Normal.
        Vector3 m_normal = Vector3(1);

        /// The roughness.
        /// @note: m_roughness has a lower priority than TX_Roughness.
        float m_roughness = 0.0f;

        /// The transparency of the material.
        /// Where 1 means opaque and 0 means totally transparent.
        /// @note: m_transparency has a lower priority than TX_Transparency.
        float m_transparency = 1.0f;

        /// Textures Map.
        std::map<ETextureType, Pointer<Texture2D>> m_textures;
    };
}