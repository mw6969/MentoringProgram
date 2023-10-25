#include <iostream>
#include <filesystem>
#include <regex>

#include "FileValidator.h"

void FileValidator::validate(std::string& fileName, const std::string_view title)
{
    passFileNameValidation(fileName, title);
    passFileExistence(fileName);
}

void FileValidator::passFileNameValidation(std::string& fileName, const std::string_view title)
{
    while(!std::regex_match(fileName, std::regex("[A-Za-z0-9]+\\.txt")))
    {
        std::cout << "Input name of a "<< title <<" file in *.txt format:";
        std::getline(std::cin, fileName);
    }
}

void FileValidator::passFileExistence(const std::string_view fileName)
{
    std::filesystem::path filePath{fileName};
    while(!std::filesystem::exists(filePath))
    {
        std::cout << "File doesn't exist. Create file manually inside current dir and press any key to continue:";
        std::string key;
        std::getline(std::cin, key);
    }
}