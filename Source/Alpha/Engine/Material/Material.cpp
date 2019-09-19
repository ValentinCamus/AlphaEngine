#include "Material.h"

namespace Alpha
{
    void Material::Bind(const Pointer<Shader>& shader, const std::string& uniformName)
    {
        Pointer<Texture2D> textureKd = GetTexture(ETextureType::TX_Diffuse);
        if (textureKd)
        {
            textureKd->Bind(0);
            shader->SetUniform(uniformName + ".tex.kd", textureKd->GetSlot());
            shader->SetUniform(uniformName + ".tex.hasKd", true);
        }
        else
        {
            shader->SetUniform(uniformName + ".tex.hasKd", false);
            shader->SetUniform(uniformName + ".kd", m_kd);
        }
        
        Pointer<Texture2D> textureKs = GetTexture(ETextureType::TX_Specular);
        if (textureKs)
        {
            textureKs->Bind(1);
            shader->SetUniform(uniformName + ".tex.ks", textureKs->GetSlot());
            shader->SetUniform(uniformName + ".tex.hasKs", true);
        }
        else
        {
            shader->SetUniform(uniformName + ".ks", m_ks);
            shader->SetUniform(uniformName + ".tex.hasKs", false);
        }

        Pointer<Texture2D> textureRough = GetTexture(ETextureType::TX_Roughness);
        if (textureRough)
        {
            textureRough->Bind(2);
            shader->SetUniform(uniformName + ".tex.roughness", textureRough->GetSlot());
            shader->SetUniform(uniformName + ".tex.hasRoughness", true);
        }
        else
        {
            shader->SetUniform(uniformName + ".roughness", m_roughness);
            shader->SetUniform(uniformName + ".tex.hasRoughness", false);
        }

        Pointer<Texture2D> textureNormal = GetTexture(ETextureType::TX_Normal);
        if (textureNormal)
        {
            textureNormal->Bind(3);
            shader->SetUniform(uniformName + ".tex.normal", textureNormal->GetSlot());
            shader->SetUniform(uniformName + ".tex.hasNormal", true);
        }
        else
        {
            // shader->SetUniform(uniformName + ".normal", m_normal);
            shader->SetUniform(uniformName + ".tex.hasNormal", false);
        }

        Pointer<Texture2D> textureMetallic = GetTexture(ETextureType::TX_Metallic);
        if (textureMetallic)
        {
            textureMetallic->Bind(4);
            shader->SetUniform(uniformName + ".tex.metallic", textureMetallic->GetSlot());
            shader->SetUniform(uniformName + ".tex.hasMetallic", true);
        }
        else
        {
            shader->SetUniform(uniformName + ".metallic", m_metallic);
            shader->SetUniform(uniformName + ".tex.hasMetallic", false);
        }

        Pointer<Texture2D> textureAO = GetTexture(ETextureType::TX_AmbientOcclusion);
        if (textureAO)
        {
            textureAO->Bind(5);
            shader->SetUniform(uniformName + ".tex.ao", textureAO->GetSlot());
            shader->SetUniform(uniformName + ".tex.hasAO", true);
        }
        else
        {
            shader->SetUniform(uniformName + ".tex.hasAO", false);
        }

        Pointer<Texture2D> textureTransparency = GetTexture(ETextureType::TX_Transparency);
        if (textureTransparency)
        {
            textureTransparency->Bind(6);
            shader->SetUniform(uniformName + ".tex.transparency", textureTransparency->GetSlot());
            shader->SetUniform(uniformName + ".tex.hasTransparency", true);
        }
        else
        {
            shader->SetUniform(uniformName + ".transparency", m_transparency);
            shader->SetUniform(uniformName + ".tex.hasTransparency", false);
        }
    }

    void Material::Unbind()
    {
        for (auto& node : m_textures)
        {
            Pointer<Texture2D>& texture = node.second;
            texture->Unbind();
        }
    }
}

