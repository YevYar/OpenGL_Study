#include "sceneObject.h"

namespace ogls::renderer
{
SceneObject::SceneObject(std::shared_ptr<ogls::oglCore::vertex::VertexArray>   vao,
                         std::shared_ptr<ogls::oglCore::shader::ShaderProgram> shaderProgram) :
    m_vao{std::move(vao)},
    m_shaderProgram{std::move(shaderProgram)}
{
}

void SceneObject::setTexturesConfiguration(ogls::oglCore::texture::TexturesConfiguration texturesConfiguration)
{
    m_texturesConfiguration = std::move(texturesConfiguration);
}

void SceneObject::applyTexturesConfiguration()
{
    ogls::oglCore::texture::applyTexturesConfiguration(m_texturesConfiguration);
}

}  // namespace ogls::renderer
