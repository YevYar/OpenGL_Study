#include "helpers.h"

#include <filesystem>
#include <fstream>
#include <iostream>

std::string helpers::readStringFromFile(std::string pathToFile)
{
    if (!std::filesystem::exists(pathToFile))
    {
        std::cerr << "File does not exist at path " << pathToFile << std::endl;
        return "";
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
        std::cerr << "File loading error (path: " << pathToFile << "): " << exc.what() << std::endl;
        return "";
    }

    return stream.str();
}
