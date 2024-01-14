#include "helpers/helpers.h"

#include <filesystem>
#include <format>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "exceptions.h"

namespace ogls::helpers
{
std::string readTextFromFile(std::string_view pathToFile)
{
    using namespace ogls;


    if (!std::filesystem::exists(pathToFile))
    {
        const auto excMes = std::format("File does not exist at path {}.", pathToFile);
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
        const auto excMes = std::format("File loading error (path: {}): {}", pathToFile, exc.what());
        throw exceptions::FileReadingException{excMes};
    }

    return stream.str();
}

std::unique_ptr<ogls::oglCore::texture::TextureData> readTextureFromFile(std::string_view pathToFile)
{
    using namespace oglCore::texture;
    using namespace ogls;


    if (!std::filesystem::exists(pathToFile))
    {
        const auto excMes = std::format("Image does not exist at path {}.", pathToFile);
        throw exceptions::FileOpeningException{excMes};
    }

    stbi_set_flip_vertically_on_load(true);
    auto width = int{0}, height = int{0}, nChannels = int{0};

    const auto txtDataDeleter = [](unsigned char* textureData)
    {
        try
        {
            stbi_image_free(textureData);
            textureData = nullptr;
        }
        catch (...)
        {
        }
    };

    auto data = TextureData::DataType{stbi_load(pathToFile.data(), &width, &height, &nChannels, 0), txtDataDeleter};

    if (!data)
    {
        const auto excMes = std::format("Cannot read an image at path {}.", pathToFile);
        throw exceptions::FileReadingException{excMes};
    }
    // TODO: image format auto detection
    return std::make_unique<TextureData>(std::move(data), width, height, nChannels, TexturePixelFormat::Rgb);
}

}  // namespace ogls::helpers
