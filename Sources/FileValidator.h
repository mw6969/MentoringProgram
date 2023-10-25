#ifndef _FILEVALIDATOR_
#define _FILEVALIDATOR_

#include <string>

class FileValidator
{
public:
    static void validate(std::string& fileName, const std::string_view title);

private:
    static void passFileNameValidation(std::string& fileName, const std::string_view title);
    static void passFileExistence(const std::string_view fileName);
};

#endif