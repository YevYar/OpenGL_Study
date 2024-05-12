#include "multicoloredRectangle.h"

#include "helpers/helpers.h"

namespace app
{
void MulticoloredRectangle::setColorCoefficient(float k)
{
    // TODO:
    if (k >= 0.0 && !isgreater(k, 1.0))
    {
        m_colorCoefficient.setValue(k);
        return;
    }
    // throw std::out_of_range{"k must be in the range [0; 1]."};
}

void MulticoloredRectangle::render(std::shared_ptr<ogls::renderer::Renderer> /*renderer*/)
{
    using namespace ogls::helpers;
    using namespace ogls::oglCore;


    if (m_counter++ == 300)
    {
        auto textureData =
          std::shared_ptr<texture::TextureData>{readTextureFromFile("resources/textures/awesomeface.png")};
        textureData->format = texture::TexturePixelFormat::Rgba;
        m_mainTexture.setData(textureData);
    }

    OGLS_GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
}

std::unique_ptr<MulticoloredRectangle> makeMulticoloredRectangle()
{
    using namespace ogls;
    using namespace ogls::mathCore;
    using namespace ogls::oglCore::texture;
    using namespace ogls::renderer;


    static auto callCounter = int{0};

    // Create the Mesh only once
    static auto rectangleMesh = std::shared_ptr<Mesh>{};

    if (callCounter == 0)
    {
        auto vertices = std::vector<Mesh::Vertex>(4);
        {
            auto topLeft = Mesh::Vertex{
              .normal    = mathCore::Vec3{1.0f, 1.0f, 1.0f},
              .otherData = {{"color", {1.0f, 1.0f, 1.0f}}},
              .position  = mathCore::Point<float>{-0.5f, -0.5f},
              .texCoords = mathCore::Point<float>{0.0f, 0.0f}
            };
            vertices[0] = std::move(topLeft);

            auto topRight = Mesh::Vertex{
              .normal    = mathCore::Vec3{1.0f, 1.0f, 1.0f},
              .otherData = {{"color", {1.0f, 0.0f, 0.0f}}},
              .position  = mathCore::Point<float>{-0.5f, 0.5f},
              .texCoords = mathCore::Point<float>{0.0f, 1.0f}
            };
            vertices[1] = std::move(topRight);

            auto bottomRight = Mesh::Vertex{
              .normal    = mathCore::Vec3{1.0f, 1.0f, 1.0f},
              .otherData = {{"color", {0.0f, 1.0f, 0.0f}}},
              .position  = mathCore::Point<float>{0.5f, 0.5f},
              .texCoords = mathCore::Point<float>{1.0f, 1.0f}
            };
            vertices[2] = std::move(bottomRight);

            auto bottomLeft = Mesh::Vertex{
              .normal    = mathCore::Vec3{1.0f, 1.0f, 1.0f},
              .otherData = {{"color", {0.0f, 0.0f, 1.0f}}},
              .position  = mathCore::Point<float>{0.5f, -0.5f},
              .texCoords = mathCore::Point<float>{1.0f, 0.0f}
            };
            vertices[3] = std::move(bottomLeft);
        }

        auto indices = std::vector<unsigned int>{0, 1, 2, 2, 3, 0};

        rectangleMesh = std::make_shared<Mesh>(std::move(vertices), std::move(indices));
    }

    // Load the texture only once
    static auto textureData =
      std::shared_ptr<TextureData>{helpers::readTextureFromFile("resources/textures/wooden_container.jpg")};

    // Create new Material
    const auto material = Material{"resources/shaders/vs/vertexShader.vert",
                                   "resources/shaders/fs/fragmentShader.frag",
                                   {{"k", 0}},
                                   {{"mainTexture", textureData}}};

    ++callCounter;

    // Create new MulticoloredRectangle
    auto rect = new MulticoloredRectangle{
      rectangleMesh, material, SceneObject::Size{.length = 100, .height = 100}
    };

    // Set the Material variables
    rect->m_colorCoefficient = material.getShaderVariable("k");
    rect->m_mainTexture      = material.getTexture("mainTexture");
    return std::unique_ptr<MulticoloredRectangle>(rect);
}

}  // namespace app
