#include "helpers/helpers.h"

#include <filesystem>
#include <format>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "exceptions.h"

std::string helpers::readStringFromFile(const std::string& pathToFile)
{
    if (!std::filesystem::exists(pathToFile))
    {
        const auto excMes = std::format("File does not exist at path {}.", pathToFile);
        throw exceptions::FileOpeningException(excMes);
    }

    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    std::stringstream stream;
    try
    {
        file.open(pathToFile, std::ios_base::in);
        stream << file.rdbuf();
        file.close();
    }
    catch (std::exception exc)
    {
        file.close();
        const auto excMes = std::format("File loading error (path: {}): {}", pathToFile, exc.what());
        throw exceptions::FileReadingException(excMes);
    }

    return stream.str();
}

std::unique_ptr<texture::TextureData> helpers::readTextureFromFile(const std::string& pathToFile)
{
    if (!std::filesystem::exists(pathToFile))
    {
        const auto excMes = std::format("Image does not exist at path {}.", pathToFile);
        throw exceptions::FileOpeningException(excMes);
    }

    stbi_set_flip_vertically_on_load(true);
    int width = 0, height = 0, nChannels = 0;
    unsigned char* data = stbi_load(pathToFile.c_str(), &width, &height, &nChannels, 0);

    if (!data)
    {
        const auto excMes = std::format("Cannot read an image at path {}.", pathToFile);
        throw exceptions::FileReadingException(excMes);
    }

    return std::make_unique<texture::TextureData>(data, width, height, nChannels);
}

void helpers::freeTextureData(texture::TextureData& textureData)
{
    stbi_image_free(textureData.getData());
}
