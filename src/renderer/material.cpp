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

Material::Variable Material::getShaderVariable(std::string shaderVariableName) const noexcept
{
    return Variable{const_cast<Material*>(this), std::move(shaderVariableName)};
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

Material::Texture Material::getTexture(std::string textureName) const noexcept
{
    return Texture{const_cast<Material*>(this), std::move(textureName)};
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

void Material::render(std::shared_ptr<Renderer> /*renderer*/)
{
    // For example, take openglCore::texture::Texture by Material::m_id and the name of texture from Renderer to
    // set some specific texture parameters.
}

Material::Texture::Texture(Material* parent, std::string textureName) noexcept :
    m_parent{parent}, m_textureName{std::move(textureName)}
{
}

std::shared_ptr<oglCore::texture::TextureData> Material::Texture::getData() const
{
    return m_parent ? m_parent->getTextureData(m_textureName) : nullptr;
}

void Material::Texture::setData(std::shared_ptr<oglCore::texture::TextureData> data)
{
    if (m_parent)
    {
        m_parent->setTextureData(m_textureName, data);
    }
}

Material::Variable::Variable(Material* parent, std::string variableName) noexcept :
    m_parent{parent}, m_variableName{std::move(variableName)}
{
}

std::variant<OGLS_SHADER_VARIABLE_TYPES> Material::Variable::getValue() const
{
    return m_parent ? m_parent->getShaderVariableValue(m_variableName) : std::variant<OGLS_SHADER_VARIABLE_TYPES>{};
}

void Material::Variable::setValue(std::variant<OGLS_SHADER_VARIABLE_TYPES> value)
{
    if (m_parent)
    {
        m_parent->setShaderVariableValue(m_variableName, std::move(value));
    }
}

}  // namespace ogls::renderer
