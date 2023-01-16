#include <glad/glad.h>

#include "multicoloredRectangle.h"

#include <exception>

#include "buffer.h"
#include "generalTypes.h"
#include "helpers/debugHelpers.h"
#include "helpers/helpers.h"
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
	// TODO: Make variable static to not create it every time for the same objects

	using namespace helpers;
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

	// Configure VAO, VBO and EBO
	auto VAO = std::make_shared<VertexArray>();

	VertexAttribute coordinateMap(0, 2, VertexAttrType::FLOAT, false, 0),
		colorMap(1, 3, VertexAttrType::FLOAT, false, getByteSizeOfType(VertexAttrType::FLOAT) * 3);

	VertexBufferLayout layout;
	layout.addVertexAttribute(coordinateMap);
	layout.addVertexAttribute(colorMap);

	ArrayData data{ reinterpret_cast<const void*>(points), sizeof(points) };
	auto VBO = std::make_shared<Buffer>(BufferTarget::ARRAY_BUFFER, data, BufferDataUsage::STATIC_DRAW, layout);

	VAO->addBuffer(VBO);

	auto EBO = std::make_shared<Buffer>(BufferTarget::ELEMENT_ARRAY_BUFFER,
		ArrayData{ reinterpret_cast<const void*>(indices), sizeof(indices) }, BufferDataUsage::STATIC_DRAW);
	VAO->addBuffer(EBO);

	// Configure shaders
	auto vShaderSource = readStringFromFile("shaders/vs/vertexShader.vert");
	auto fShaderSource = readStringFromFile("shaders/fs/fragmentShader.frag");
	if (vShaderSource.empty() || fShaderSource.empty())
	{
		throw std::runtime_error("Vertex or fragment shader source is empty.");
	}

	Shader vShader(ShaderType::VERTEX_SHADER, vShaderSource),
		fShader(ShaderType::FRAGMENT_SHADER, fShaderSource);

	if (!vShader.isValid() || !fShader.isValid())
	{
		throw std::runtime_error("Vertex or fragment shader cannot be created.");
	}

	auto shaderProgram = std::make_shared<ShaderProgram>(vShader, fShader);
	if (!shaderProgram->isValid())
	{
		throw std::runtime_error("Shader program cannot be compiled.");
	}

	auto rect = new MulticoloredRectangle(VAO, shaderProgram);
	return std::unique_ptr<MulticoloredRectangle>(rect);
}
