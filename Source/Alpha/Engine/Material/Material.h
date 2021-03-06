#pragma once

#include <Alpha/Core/CoreMinimal.h>

#include <Alpha/Engine/Renderer/Shader.h>
#include <Alpha/Engine/Renderer/Texture.h>

namespace Alpha
{
    enum class EMaterialType
    {
        Opaque = 0,
        Transparent = 1
    };

    /// The type of the texture.
    enum class ETextureType
    {
        TX_Diffuse,
        TX_Specular,
        TX_Normal,
        TX_Roughness,
        TX_Metallic,
        TX_AmbientOcclusion,
        TX_Transparency
    };

    class Material
    {
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
        /// @getter: The name of the material.
        inline const std::string& GetName() const { return m_name; }

        inline EMaterialType GetType() const { return m_type; }

        inline void SetType(const EMaterialType& type) { m_type = type; }

        /// @getter: The Kd color of the material.
        inline const Vector4& GetKd() const { return m_kd; }

        /// @setter: The Kd color of the material.
        inline void SetKd(const Vector4& kd) { m_kd = kd; }

        /// @getter: The Ks color of the material.
        inline const Vector4& GetKs() const { return m_ks; }

        /// @setter: The Ks color of the material.
        inline void SetKs(const Vector4& ks) { m_ks = ks; }

        /// @getter: The roughness of the material.
        inline float GetRoughness() const { return m_roughness; }

        /// @setter: The roughness of the material.
        inline void SetRoughness(float roughness) { m_roughness = roughness; }

        /// @getter: The metallic of the material.
        inline float GetMetallic() const { return m_metallic; }

        /// @setter: The metallic of the material.
        inline void SetMetallic(float metallic) { m_metallic = metallic; }

        /// @getter: The transparency of the material.
        inline float GetTransparency() const { return m_transparency; }

        /// @setter: The transparency of the material.
        inline void SetTransparency(float transparency) { m_transparency = transparency; }

    private:
        /// The name of the uniform.
        std::string m_name;

        /// Is this material opaque or transparent?
        EMaterialType m_type = EMaterialType::Transparent;

        /// The Kd (diffuse) color.
        /// @note: m_kd color has a lower priority than TX_Diffuse.
        Vector4 m_kd = Vector4(1);

        /// The Ks (specular) color.
        /// @note: m_ks color has a lower priority than TX_Specular.
        Vector4 m_ks = Vector4(1);

        /// The roughness.
        /// @note: m_roughness has a lower priority than TX_Roughness.
        float m_roughness = 0.2f;

        /// The metallic aspect of the material.
        /// @note: m_transparency has a lower priority than TX_Transparency.
        float m_metallic = 0.1f;

        /// The transparency of the material.
        /// Where 1 means opaque and 0 means totally transparent.
        /// @note: m_transparency has a lower priority than TX_Transparency.
        float m_transparency = 1.0f;

        /// Textures Map.
        std::map<ETextureType, Pointer<Texture2D>> m_textures;
    };
}