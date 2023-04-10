#include "helpers/helpers.h"

#include <filesystem>
#include <format>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "exceptions.h"

namespace ogls::helpers
{
void freeTextureData(ogls::oglCore::texture::TextureData& textureData)
{
    stbi_image_free(textureData.m_data);
}

std::string readStringFromFile(const std::string& pathToFile)
{
    using namespace ogls;


    if (!std::filesystem::exists(pathToFile))
    {
        const auto excMes = std::format{"File does not exist at path {}.", pathToFile};
        throw exceptions::FileOpeningException{excMes};
    }

    auto file = std::ifstream{};
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    auto stream = std::stringstream{};
    try
    {
        file.open(pathToFile, std::ios_base::in);
        stream << file.rdbuf();
        file.close();
    }
    catch (const std::exception& exc)
    {
        file.close();
        const auto excMes = std::format{"File loading error (path: {}): {}", pathToFile, exc.what()};
        throw exceptions::FileReadingException{excMes};
    }

    return stream.str();
}

std::unique_ptr<ogls::oglCore::texture::TextureData> readTextureFromFile(const std::string& pathToFile)
{
    using namespace ogls;


    if (!std::filesystem::exists(pathToFile))
    {
        const auto excMes = std::format{"Image does not exist at path {}.", pathToFile};
        throw exceptions::FileOpeningException{excMes};
    }

    stbi_set_flip_vertically_on_load(true);
    auto width     = int{0};
    auto height    = int{0};
    auto nChannels = int{0};
    auto data      = stbi_load(pathToFile.c_str(), &width, &height, &nChannels, 0);

    if (!data)
    {
        const auto excMes = std::format{"Cannot read an image at path {}.", pathToFile};
        throw exceptions::FileReadingException{excMes};
    }
    // TODO: image format auto detection
    return std::make_unique<oglCore::texture::TextureData>(data, width, height, nChannels,
                                                           oglCore::texture::TexturePixelFormat::RGB);
}

}  // namespace ogls::helpers
