#include "helpers/helpers.h"

#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>

#include "exceptions.h"

std::string helpers::readStringFromFile(const std::string& pathToFile)
{
    if (!std::filesystem::exists(pathToFile))
    {
        const auto excMes = std::format("File does not exist at path {}", pathToFile);
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
