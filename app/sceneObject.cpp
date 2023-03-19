#include "sceneObject.h"

using namespace renderer;

SceneObject::SceneObject(std::shared_ptr<openglCore::vertex::VertexArray> vao,
	std::shared_ptr<openglCore::shader::ShaderProgram> shaderProgram) :
	m_vao{ std::move(vao) }, m_shaderProgram{ std::move(shaderProgram) }
{
}

void SceneObject::setTexturesConfiguration(openglCore::texture::TexturesConfiguration texturesConfiguration)
{
    m_texturesConfiguration = std::move(texturesConfiguration);
}

void SceneObject::applyTexturesConfiguration()
{
    openglCore::texture::applyTexturesConfiguration(m_texturesConfiguration);
}
