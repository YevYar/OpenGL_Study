#include "texture.h"

#include <glad/glad.h>

#include "exceptions.h"
#include "helpers/helpers.h"
#include "helpers/debugHelpers.h"

using namespace texture;

Texture::Texture(TextureBindingTarget target, std::shared_ptr<TextureData> textureData) : m_target{ target },
    m_data{ std::move(textureData) }
{
    genTexture();
}

Texture::Texture(const Texture& obj) : m_target{ obj.m_target }, m_data{ obj.m_data }
{
    genTexture();
}

Texture::Texture(Texture&& obj) noexcept : m_rendererId{ obj.m_rendererId }, m_target{ obj.m_target },
    m_data{ std::move(obj.m_data) }, m_isDataSet{ obj.m_isDataSet }
{
    obj.m_rendererId = 0;
    obj.m_isDataSet = true;
}

Texture::~Texture()
{
    GLCall(glDeleteTextures(1, &m_rendererId));
}

Texture& Texture::operator=(const Texture& obj)
{
    m_

}

// Texture::Texture(TextureTarget target, std::shared_ptr<TextureData> textureData)

void Texture::genTexture()
{
    GLCall(glGenTextures(1, &m_rendererId));
    if (m_rendererId == 0)
    {
        throw exceptions::GLRecAcquisitionException("Texture cannot be generated.");
    }
}
