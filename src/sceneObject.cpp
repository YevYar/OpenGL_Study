#include "sceneObject.h"

using namespace renderer;

SceneObject::SceneObject(std::shared_ptr<vertex::VertexArray> vao,
	std::shared_ptr<shader::ShaderProgram> shaderProgram) :
	m_vao{ std::move(vao) }, m_shaderProgram{ std::move(shaderProgram) }
{
}
