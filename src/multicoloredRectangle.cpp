#include <glad/glad.h>

#include "multicoloredRectangle.h"

#include <exception>

#include "buffer.h"
#include "generalTypes.h"
#include "helpers/debugHelpers.h"
#include "shaderProgram.h"
#include "vertexBufferLayout.h"

using namespace renderer;

MulticoloredRectangle::MulticoloredRectangle(std::shared_ptr<vertex::VertexArray> vao,
	std::shared_ptr<shader::ShaderProgram> shaderProgram) :
	SceneObject(std::move(vao), shaderProgram),
	m_colorCoefficient{ static_cast<decltype(m_colorCoefficient)>(shaderProgram->findUniform<float, 1>("k")) }
{

}

void MulticoloredRectangle::draw()
{
	m_vao->bind();
	m_shaderProgram->use();
	
	GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
}

void MulticoloredRectangle::setColorCoefficient(const float* k)
{
	m_shaderProgram->use();
	m_colorCoefficient.setData(k);
}

std::unique_ptr<MulticoloredRectangle> renderer::makeMulticoloredRectangle()
{
	using namespace shader;
	using namespace vertex;

	static const float points[] = {
		-0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f
	};
	static const unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	// Configure VAO, VBO and EBO only once
	static auto VAO = std::make_shared<VertexArray>();

	static VertexAttribute coordinateMap(0, 2, VertexAttrType::FLOAT, false, 0),
		colorMap(1, 3, VertexAttrType::FLOAT, false, getByteSizeOfType(VertexAttrType::FLOAT) * 3);

	static VertexBufferLayout layout;
	layout.addVertexAttribute(coordinateMap);
	layout.addVertexAttribute(colorMap);

	static ArrayData data{ reinterpret_cast<const void*>(points), sizeof(points) };
	static auto VBO = std::make_shared<Buffer>(BufferTarget::ARRAY_BUFFER, data, BufferDataUsage::STATIC_DRAW, layout);

	VAO->addBuffer(VBO);

	static auto EBO = std::make_shared<Buffer>(BufferTarget::ELEMENT_ARRAY_BUFFER,
		ArrayData{ reinterpret_cast<const void*>(indices), sizeof(indices) }, BufferDataUsage::STATIC_DRAW);
	VAO->addBuffer(EBO);

	// Create shader program only once
	static std::shared_ptr<ShaderProgram> shaderProgram = makeShaderProgram("shaders/vs/vertexShader.vert",
		"shaders/fs/fragmentShader.frag");

	// Create new MulticoloredRectangle
	auto rect = new MulticoloredRectangle(VAO, shaderProgram);
	return std::unique_ptr<MulticoloredRectangle>(rect);
}
