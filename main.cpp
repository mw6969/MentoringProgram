#include <string>

#include "FileValidator.h"
#include "Tool.h"

int main(int argc, char *argv[])
{
    std::string sourceName, targetName;
    FileValidator::validate(sourceName, "source");
    FileValidator::validate(targetName, "target");

    Tool::copy(sourceName, targetName);

    return 0;
}