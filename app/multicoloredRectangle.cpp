#include <glad/glad.h>

#include "multicoloredRectangle.h"

#include <exception>

#include "buffer.h"
#include "generalTypes.h"
#include "helpers/debugHelpers.h"
#include "helpers/helpers.h"
#include "shaderProgram.h"
#include "texture.h"

using namespace renderer;

MulticoloredRectangle::MulticoloredRectangle(std::shared_ptr<openglCore::vertex::VertexArray> vao,
	std::shared_ptr<openglCore::shader::ShaderProgram> shaderProgram) :
	SceneObject(std::move(vao), shaderProgram),
	m_colorCoefficient{ static_cast<decltype(m_colorCoefficient)>(shaderProgram->findUniform<float, 1>("k")) }
{
}

void MulticoloredRectangle::draw()
{
    using namespace openglCore;

	m_vao->bind();
	m_shaderProgram->use();
	applyTexturesConfiguration();

	m_counter++;
	if (m_counter == 300)
	{
		auto textureData = std::shared_ptr<texture::TextureData>(
			helpers::readTextureFromFile("resources/textures/awesomeface.png"));
		textureData->m_format = texture::TexturePixelFormat::RGBA;
		texture::castBaseTextureToTexture<2>(m_texturesConfiguration, 0, 0)
			->setData(texture::TexImage2DTarget::TEXTURE_2D, textureData);
	}
	
	GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
}

void MulticoloredRectangle::setColorCoefficient(const float* k)
{
    if (*k >= 0 && *k <= 1.0)
    {
        m_shaderProgram->use();
        m_colorCoefficient.setData(k);
    }	
}

std::unique_ptr<MulticoloredRectangle> renderer::makeMulticoloredRectangle()
{
	using namespace openglCore::shader;
    using namespace openglCore::texture;
    using namespace openglCore::vertex;


	static const float points[] = {
		-0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f
	};
	static const unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	// Configure VAO, VBO and EBO only once
	static auto VAO = std::make_shared<VertexArray>();

	static VertexAttribute coordinateMap(0, 2, VertexAttrType::FLOAT, false, 0),
		colorMap(1, 3, VertexAttrType::FLOAT, false, getByteSizeOfType(VertexAttrType::FLOAT) * 2),
		texCoords(2, 2, VertexAttrType::FLOAT, false, getByteSizeOfType(VertexAttrType::FLOAT) * 5);

	static VertexBufferLayout layout;
	layout.addVertexAttribute(coordinateMap);
	layout.addVertexAttribute(colorMap);
	layout.addVertexAttribute(texCoords);

	static ArrayData data{ reinterpret_cast<const void*>(points), sizeof(points) };
	static auto VBO = std::make_shared<Buffer>(BufferTarget::ARRAY_BUFFER, data, BufferDataUsage::STATIC_DRAW, layout);

	VAO->addBuffer(VBO);

	static auto EBO = std::make_shared<Buffer>(BufferTarget::ELEMENT_ARRAY_BUFFER,
		ArrayData{ reinterpret_cast<const void*>(indices), sizeof(indices) }, BufferDataUsage::STATIC_DRAW);
	VAO->addBuffer(EBO);

	// Create shader program only once
	static std::shared_ptr<ShaderProgram> shaderProgram = makeShaderProgram("resources/shaders/vs/vertexShader.vert",
		"resources/shaders/fs/fragmentShader.frag");

	// Load textures only once
	static auto textureData = std::shared_ptr<TextureData>(
		helpers::readTextureFromFile("resources/textures/wooden_container.jpg"));
	static auto texture2D = std::make_shared<Texture<2>>(TextureTarget::TEXTURE_2D,
		TexImage2DTarget::TEXTURE_2D, textureData);
	static TexturesConfiguration texturesConfig{{
		0,
		std::vector<std::shared_ptr<BaseTexture>>{ texture2D }
		}};


	// Create new MulticoloredRectangle
	auto rect = new MulticoloredRectangle(VAO, shaderProgram);
	rect->setTexturesConfiguration(texturesConfig);
	return std::unique_ptr<MulticoloredRectangle>(rect);
}
