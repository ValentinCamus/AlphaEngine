#include "Material.h"

namespace Alpha
{
    void Material::Bind(const Pointer<Shader>& shader)
    {
        Pointer<Texture2D> textureKd = GetTexture(ETextureType::TX_Diffuse);
        if (textureKd)
        {
            textureKd->Bind(0);
            shader->SetUniform(m_name + ".tex.kd", textureKd->GetSlot());
            shader->SetUniform(m_name + ".tex.hasKd", true);
        }
        else
        {
            shader->SetUniform(m_name + ".tex.hasKd", false);
            shader->SetUniform(m_name + ".kd", m_kd);
        }

        /*
        Pointer<Texture2D> textureKs = GetTexture(TextureType::TX_Specular);
        if (textureKs)
        {
            textureKs->Bind(1);
            shader->SetUniform(m_name + ".tex.ks", textureKs->GetSlot());
            shader->SetUniform(m_name + ".tex.hasKs", true);
        }
        else
        {
            shader->SetUniform(m_name + ".ks", m_ks);
            shader->SetUniform(m_name + ".tex.hasKs", false);
        }

        Pointer<Texture2D> textureRough = GetTexture(TextureType::TX_Roughness);
        if (textureRough)
        {
            textureRough->Bind(2);
            shader->SetUniform(m_name + ".tex.roughness", textureRough->GetSlot());
            shader->SetUniform(m_name + ".tex.hasRoughness", true);
        }
        else
        {
            shader->SetUniform(m_name + ".roughness", m_roughness);
            shader->SetUniform(m_name + ".tex.hasRoughness", false);
        }

        Pointer<Texture2D> textureNormal = GetTexture(TextureType::TX_Normal);
        if (textureNormal)
        {
            textureNormal->Bind(3);
            shader->SetUniform(m_name + ".tex.normal", textureNormal->GetSlot());
            shader->SetUniform(m_name + ".tex.hasNormal", true);
        }
        else
        {
            shader->SetUniform(m_name + ".normal", m_normal);
            shader->SetUniform(m_name + ".tex.hasNormal", false);
        }

        Pointer<Texture2D> textureTransparency = GetTexture(TextureType::TX_Transparency);
        if (textureTransparency)
        {
            textureTransparency->Bind(4);
            shader->SetUniform(m_name + ".tex.transparency", textureTransparency->GetSlot());
            shader->SetUniform(m_name + ".tex.hasTransparency", true);
        }
        else
        {
            shader->SetUniform(m_name + ".transparency", m_transparency);
            shader->SetUniform(m_name + ".tex.hasTransparency", false);
        }
         */
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

