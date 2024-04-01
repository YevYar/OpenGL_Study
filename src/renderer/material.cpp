#include "material.h"

namespace ogls::renderer
{
Material::Material(std::string pathToVertexShader, std::string pathToFragmentShader,
                   std::unordered_map<std::string, std::variant<OGLS_SHADER_VARIABLE_TYPES>>       shaderVariables,
                   std::unordered_map<std::string, std::shared_ptr<oglCore::texture::TextureData>> textures) noexcept :
    m_pathToFragmentShader{std::move(pathToFragmentShader)},
    m_pathToVertexShader{std::move(pathToVertexShader)}, m_shaderVariables{std::move(shaderVariables)},
    m_textures{std::move(textures)}
{
    for (const auto& el : m_shaderVariables)
    {
        m_shaderVariablesToUpdate.push_back(el.first);
    }
    for (const auto& el : m_textures)
    {
        m_texturesToUpdate.push_back(el.first);
    }
}

Material::Color Material::getColor(std::string_view shaderVariableName) const
{
    return std::get<Color>(getShaderVariableValue(shaderVariableName));
}

std::string_view Material::getPathToFragmentShader() const noexcept
{
    return std::string_view{m_pathToFragmentShader};
}

std::string_view Material::getPathToVertexShader() const noexcept
{
    return std::string_view{m_pathToVertexShader};
}

std::variant<OGLS_SHADER_VARIABLE_TYPES> Material::getShaderVariableValue(std::string_view shaderVariableName) const
{
    return m_shaderVariables.at(std::string{shaderVariableName});
}

const std::unordered_map<std::string, std::variant<OGLS_SHADER_VARIABLE_TYPES>>&
  Material::getShaderVariables() const noexcept
{
    return m_shaderVariables;
}

std::shared_ptr<oglCore::texture::TextureData> Material::getTextureData(std::string_view textureName) const
{
    return m_textures.at(std::string{textureName});
}

const std::unordered_map<std::string, std::shared_ptr<oglCore::texture::TextureData>>&
  Material::getTextures() const noexcept
{
    return m_textures;
}

void Material::render()
{
    // For example, take openglCore::texture::Texture by Material::m_id and the name of texture from Renderer to
    // set some specific texture parameters.
}

void Material::setColor(std::string_view shaderVariableName, Color color)
{
    setShaderVariableValue(shaderVariableName, color);
}

void Material::setShaderVariableValue(std::string_view                         shaderVariableName,
                                      std::variant<OGLS_SHADER_VARIABLE_TYPES> value)
{
    m_shaderVariables.insert(std::make_pair(std::string{shaderVariableName}, value));
    m_shaderVariablesToUpdate.push_back(std::string{shaderVariableName});
}

void Material::setTextureData(std::string_view textureName, std::shared_ptr<oglCore::texture::TextureData> data)
{
    m_textures.insert(std::make_pair(std::string{textureName}, data));
    m_texturesToUpdate.push_back(std::string{textureName});
}

void Material::setUpdatedState()
{
    m_shaderVariablesToUpdate.clear();
    m_texturesToUpdate.clear();
}

}  // namespace ogls::renderer
