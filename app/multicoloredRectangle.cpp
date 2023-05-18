#include "multicoloredRectangle.h"

#include <array>
#include <cmath>
#include <exception>
#include <stdexcept>

#include <glad/glad.h>

#include "buffer.h"
#include "generalTypes.h"
#include "helpers/debugHelpers.h"
#include "helpers/helpers.h"
#include "shaderProgram.h"
#include "texture.h"
#include "uniforms.h"

namespace app
{
MulticoloredRectangle::MulticoloredRectangle(std::shared_ptr<ogls::oglCore::vertex::VertexArray>   vao,
                                             std::shared_ptr<ogls::oglCore::shader::ShaderProgram> shaderProgram) :
    SceneObject{std::move(vao), shaderProgram},
    m_colorCoefficient{static_cast<decltype(m_colorCoefficient)>(shaderProgram->findUniform<float, 1>("k"))}
{
}

void MulticoloredRectangle::setColorCoefficient(float k)
{
    // TODO:
    if (k >= 0.0 && !isgreater(k, 1.0))
    {
        m_shaderProgram->use();
        m_colorCoefficient.setData(&k);
        return;
    }
    // throw std::out_of_range{"k must be in the range [0; 1]."};
}

void MulticoloredRectangle::render()
{
    using namespace ogls::helpers;
    using namespace ogls::oglCore;


    m_vao->bind();
    m_shaderProgram->use();
    applyTexturesConfiguration();

    if (m_counter++ == 300)
    {
        auto textureData =
          std::shared_ptr<texture::TextureData>{readTextureFromFile("resources/textures/awesomeface.png")};
        textureData->format = texture::TexturePixelFormat::Rgba;
        texture::castBaseTextureToTexture<2>(m_texturesConfiguration, 0, 0)->setData(textureData);
    }

    OGLS_GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
}

std::unique_ptr<MulticoloredRectangle> makeMulticoloredRectangle()
{
    using namespace ogls;
    using namespace ogls::oglCore::shader;
    using namespace ogls::oglCore::texture;
    using namespace ogls::oglCore::vertex;


    // clang-format off
    static const auto points  = std::array<GLfloat, 28>{
		-0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f
	};
    // clang-format on
    static const auto indices = std::array<GLuint, 6>{0, 1, 2, 2, 3, 0};

    // Configure VAO, VBO and EBO only once
    static auto VAO = std::make_shared<VertexArray>();

    static auto coordinateMap =
      VertexAttribute{.byteOffset{0}, .count{2}, .index{0}, .normalized{false}, .type{VertexAttrType::Float}};
    static auto colorMap  = VertexAttribute{.byteOffset{getByteSizeOfType(VertexAttrType::Float) * 2},
                                            .count{3},
                                            .index{1},
                                            .normalized{false},
                                            .type{VertexAttrType::Float}};
    static auto texCoords = VertexAttribute{.byteOffset{getByteSizeOfType(VertexAttrType::Float) * 5},
                                            .count{2},
                                            .index{2},
                                            .normalized{false},
                                            .type{VertexAttrType::Float}};

    static auto layout = VertexBufferLayout{};
    layout.addVertexAttribute(coordinateMap);
    layout.addVertexAttribute(colorMap);
    layout.addVertexAttribute(texCoords);

    static auto data = ArrayData{reinterpret_cast<const void*>(points.data()), sizeof(points)};
    static auto VBO  = std::make_shared<Buffer>(BufferTarget::ArrayBuffer, data, BufferDataUsage::StaticDraw, layout);

    VAO->addBuffer(VBO);

    static auto EBO =
      std::make_shared<Buffer>(BufferTarget::ElementArrayBuffer,
                               ArrayData{reinterpret_cast<const void*>(indices.data()), sizeof(indices)},
                               BufferDataUsage::StaticDraw);
    VAO->addBuffer(EBO);

    // Create shader program only once
    static auto shaderProgram = std::shared_ptr<ShaderProgram>{
      makeShaderProgram("resources/shaders/vs/vertexShader.vert", "resources/shaders/fs/fragmentShader.frag")};

    // Load textures only once
    static auto textureData =
      std::shared_ptr<TextureData>{helpers::readTextureFromFile("resources/textures/wooden_container.jpg")};
    static auto texture2D = std::make_shared<Texture<2>>(TextureTarget::Texture2d, textureData);
    static TexturesConfiguration texturesConfig{
      {0, std::vector<std::shared_ptr<BaseTexture>>{texture2D}}
    };

    // Create new MulticoloredRectangle
    auto rect = new MulticoloredRectangle{VAO, shaderProgram};
    rect->setTexturesConfiguration(texturesConfig);
    return std::unique_ptr<MulticoloredRectangle>(rect);
}

}  // namespace app
